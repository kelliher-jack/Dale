#include "stdio.h"
#include "dale.h"
#include "hash.h"
/* lists.h is included here for LIST macros, but it also defines inline
 * functions that end up in eval.o.  expr.c must NOT be linked because
 * those same functions would then be duplicated. */
#include "lists.h"

OBJPTR findprimary();
OBJPTR evalexpr();
OBJPTR evalblock();

/* MAXPARAMS and MAXLOCALS are now defined in dale.h */

/* OBJPTR (*procedure[])();  is the array of primitive C procedures */  
/*          for dale primitives.  */


/* conventions  parameters[0] always refers to self */
/* assume that strings only refer to class or function objects */
/*   all other vars have been resolved into local var reference objects, */
/*   parameter reference objects or instance var reference  objects. */

OBJPTR eval(object,parameters,locals)
        OBJPTR *parameters[];
        OBJPTR *locals[];
        OBJPTR *object;

{
        OBJPTR *class;

        if(object == NULL) {
                printf("expression evaluated to NULL\n");
                return(NULL);
        }
        if((class=ClassSymbol(object)) == dSYMBOL) {
                if(object[OBJPR] == NULL)
                        printf("unbound symbol %s\n",object[STRPTR]);
                return(object[OBJPR]);
        }
        else if (class == dVAR) {
                        OBJPTR *addr;
                addr = NewInstance(dADDR,POINTERSIZE);
                addr[0] = &locals[(int)object[1]];
                return(addr);
        }
        else if (class == dADDR) {
                        OBJPTR *address;
            /* eval object at that address */
                address = object[0];
                return(eval(*address,parameters,locals));
        }
        else if (class == dPARAMETER) {
                return(parameters[(int)object[1]]);
        }
        else if (class == dEXPR) {
                return(evalexpr(ExprToList(object),locals));
        }
        else if (class == dEXPRLIST) {
                return(evalexpr(object,locals));
        }
        else if (class == dPRIM) {
                return((*procedure[(int)(long)object[0]])(parameters));
        }
        else if (class == dBLOCK) {
                return(evalblock(object,parameters,locals));
        }
        else {
                return(object);
        }
}
                
OBJPTR evalexpr(expr,locals)
        LIST expr;
        OBJPTR *locals;  /* local vars environment  */

{
        OBJPTR *methodtemplate,*methodblock,*class,*classym;
        OBJPTR *parameters[MAXPARAMS],*newlocals[MAXLOCALS];
        OBJPTR *primarymsg;
        LIST tmp;
        int i,env;


        /* eval self to an object and assign to parameters[0] which     */
        /* always refers to self.                                                                       */
        /* Note the first link in a expression may have a garbage   */
        /* P or PREV pointer.                                                                           */
        /* environments don't change at invocation of a expression  */
        /* the environment is only changed when we enter a new          */
        /* class.                                                                                                       */

        parameters[0]=eval(GetValPtr(expr),parameters,locals);
        tmp = expr;
        expr = NEXT(expr);  /* strip off subject */
        FreeInstance(tmp);

        /* single object expressions (return the object) */
        while (expr != NULL) {
                primarymsg = GetValPtr(expr);
                class = GetClass(parameters[0]);
                classym = ClassSymbol(parameters[0]);
                if(classym == dCLASS) {
                        env = XCLASS;
                        class = parameters[0]; /* self is a class */
                }
                else env = EXTERNAL;
                primarymsg=(GetValPtr(expr));
                /* special case for assignment operater and addresses */
                if (primarymsg == dASIGN) {
                        if (classym != dADDR) {
                                printf("illegal assignment must be a lval\n");
                                return(NULL);
                        }
                }
                else if (classym == dADDR) {
                        OBJPTR *address;
                    /* eval object at that address */
                        address = parameters[0][0];
                        if(*address == NULL) {
                                printf("unbound variable\n");
                                return(NULL);
                        }
                        parameters[0] = eval(*address,parameters,locals);
                        if(parameters[0] == NULL) {
                                printf("unbound variable\n");
                                return(NULL);
                        }
                        class = GetClass(parameters[0]);
                }
                if ((methodtemplate = findprimary(class,primarymsg[STRPTR],
                                                  &methodblock,env)) == NULL) {
                        printf("primary message %s not found for class %s\n",
                                        primarymsg[STRPTR], class ? (char*)class[5] : "(unknown)");
                        return(NULL);
                }
                if (methodtemplate[1])
                        if(methodtemplate[4]) {
                                printf("n-ary parameters not implemented yet\n");
                                return(NULL);
                        }
                        else {  /* binary operater */
                                tmp = expr;
                                if((expr=NEXT(expr)) == NULL) {
                                        printf("missing parameter for %s message\n",
                                                        primarymsg[STRPTR]);
                                        FreeInstance(tmp);
                                        return(NULL); /* make a error object rem free list */
                                }
                                FreeInstance(tmp);
                                /* eval parameter in current environment */
                                parameters[1] = eval(GetValPtr(expr),parameters,locals);
                        if(ClassSymbol(parameters[1])==dADDR)
                                parameters[1] = eval(parameters[1],parameters,locals);
                                tmp = expr;
                                expr=NEXT(expr);
                                FreeInstance(tmp);
                                /* eval method in a new environment */
                                for(i=0;i<MAXLOCALS;i++)
                                        newlocals[i] = NULL;
                                parameters[0] = eval(methodblock,parameters,newlocals);
                        }
                else {  /* template consists of only a primary message */
                        tmp = expr;
                        expr=NEXT(expr);
                        FreeInstance(tmp);
                        for(i=0;i<MAXLOCALS;i++)
                                newlocals[i] = NULL;
                        parameters[0] = eval(methodblock,parameters,newlocals);
                }
        }
        return(parameters[0]);
}

OBJPTR evalblock(block,parameters,locals)
        OBJPTR *block,parameters[],locals[]; 

        /* environments don't change in invocation of a block */

{
        int ExprPtr;
        OBJPTR result;

        for(ExprPtr=2; ExprPtr < (int)block[0] + 2; ExprPtr++) {
                result = eval(block[ExprPtr],parameters,locals);
        }
        return(result);
}

OBJPTR findprimary(aClass,PrimaryMsg,rtnblock,env)
        OBJPTR *aClass;
        OBJPTR PrimaryMsg;
        OBJPTR *rtnblock;
        int env;
{
        OBJPTR *ptr,*tmp;
        OBJPTR *super;
        int i;

        /* look through external methods only for now */

        if (aClass == NULL) return(NULL); /* debug only besure its their */

        ptr = aClass[env];
        if(ptr != NULL) {
                for(i = 1; i < (int)ptr[0]*2+1; i+=2) {
                        tmp=ptr[i];  /* tmp=ptr to template */
                        if(strcmp(PrimaryMsg,tmp[0]) == 0) {
                                *rtnblock = ptr[i+1];
                                return(tmp);
                        }
                }
        }
        /* not found in this class — follow superclass chain */
        super = aClass[0];
        if(super == NULL) return(NULL);
        if(super[OBJPR] == NULL) return(NULL);
        return(findprimary(super[OBJPR],PrimaryMsg,rtnblock,env));
}
