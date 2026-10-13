#include "stdio.h"
#include "dale.h"

extern void dump(OBJPTR table[]);

OBJPTR iadd(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self,*ptr,*anInteger;

        self = parameters[0];
        anInteger = parameters[1];
        ptr = NewInstance(dINTEGER,POINTERSIZE);
        ptr[0] = (OBJPTR)((int)self[0] + (int)anInteger[0]);
        return(ptr);
}
        
OBJPTR isub(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self,*ptr,*anInteger;

        self = parameters[0];
        anInteger = parameters[1];
        ptr = NewInstance(dINTEGER,POINTERSIZE);
        ptr[0] = (OBJPTR)((int)self[0] - (int)anInteger[0]);
        return(ptr);
}
        
OBJPTR imult(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self,*ptr,*anInteger;

        self = parameters[0];
        anInteger = parameters[1];
        ptr = NewInstance(dINTEGER,POINTERSIZE);
        ptr[0] = (OBJPTR)((int)self[0] * (int)anInteger[0]);
        return(ptr);
}
        
OBJPTR idiv(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self,*ptr,*anInteger;

        self = parameters[0];
        anInteger = parameters[1];
        ptr = NewInstance(dINTEGER,POINTERSIZE);
        ptr[0] = (OBJPTR)((int)self[0] / (int)anInteger[0]);
        return(ptr);
}

OBJPTR imod(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self,*ptr,*anInteger;

        self = parameters[0];
        anInteger = parameters[1];
        ptr = NewInstance(dINTEGER,POINTERSIZE);
        ptr[0] = (OBJPTR)((int)self[0] % (int)anInteger[0]);
        return(ptr);
}

OBJPTR iequ(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self,*anInteger;
        BOOLEAN *ptr;
        self = parameters[0];
        anInteger = parameters[1];
        ptr = (BOOLEAN *)NewInstance(dBOOLEAN,POINTERSIZE);
        ptr[0] = (BOOLEAN)((int)self[0] == (int)anInteger[0]);
        return((OBJPTR)ptr);
}

OBJPTR inotequ(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self,*anInteger;
        BOOLEAN *ptr;
        self = parameters[0];
        anInteger = parameters[1];
        ptr = (BOOLEAN *)NewInstance(dBOOLEAN,POINTERSIZE);
        ptr[0] = (BOOLEAN)((int)self[0] != (int)anInteger[0]);
        return((OBJPTR)ptr);
}

OBJPTR iless(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self,*anInteger;
        BOOLEAN *ptr;
        self = parameters[0];
        anInteger = parameters[1];
        ptr = (BOOLEAN *)NewInstance(dBOOLEAN,POINTERSIZE);
        ptr[0] = (BOOLEAN)((int)self[0] < (int)anInteger[0]);
        return((OBJPTR)ptr);
}

OBJPTR ilessequ(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self,*anInteger;
        BOOLEAN *ptr;
        self = parameters[0];
        anInteger = parameters[1];
        ptr = (BOOLEAN *)NewInstance(dBOOLEAN,POINTERSIZE);
        ptr[0] = (BOOLEAN)((int)self[0] <= (int)anInteger[0]);
        return((OBJPTR)ptr);
}

OBJPTR igreater(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self,*anInteger;
        BOOLEAN *ptr;
        self = parameters[0];
        anInteger = parameters[1];
        ptr = (BOOLEAN *)NewInstance(dBOOLEAN,POINTERSIZE);
        ptr[0] = (BOOLEAN)((int)self[0] > (int)anInteger[0]);
        return((OBJPTR)ptr);
}

OBJPTR inotless(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self,*anInteger;
        BOOLEAN *ptr;
        self = parameters[0];
        anInteger = parameters[1];
        ptr = (BOOLEAN *)NewInstance(dBOOLEAN,POINTERSIZE);
        ptr[0] = (BOOLEAN)((int)self[0] >= (int)anInteger[0]);
        return((OBJPTR)ptr);
}

OBJPTR iincr(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *ptr;
        OBJPTR *self;

        self = parameters[0];
        ptr = NewInstance(dINTEGER,POINTERSIZE);
        ptr[0] = (OBJPTR)((int)self[0] + 1);
        return(ptr);
}

OBJPTR idecr(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *ptr;
        OBJPTR *self;

        self = parameters[0];
        ptr = NewInstance(dINTEGER,POINTERSIZE);
        ptr[0] = (OBJPTR)((int)self[0] - 1);
        return(ptr);
}

OBJPTR iprint(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self;

        self = parameters[0];
        printf("%d",(int)self[0]);
        return(parameters[0]);
}

OBJPTR ilprint(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self;

        self = parameters[0];
        printf("%d\n",(int)self[0]);
        return(parameters[0]);
}

OBJPTR bprint(parameters)
        OBJPTR *parameters[];
{
        BOOLEAN *self;
        self = (BOOLEAN *)parameters[0];
        if(*self) printf("TRUE");
        else printf("FALSE");
        return(parameters[0]);
}

OBJPTR slprint(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self;

        self = parameters[0];
        printf("%s\n",self);
        return(parameters[0]);
}

OBJPTR oprint(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *self;
        OBJPTR class;

        self = parameters[0];
        class = ClassSymbol(self);
        if(class == dSTRING)
                printf("%s",(char *)self);
        else if(class == dINTEGER)
                printf("%d",(int)(long)self[0]);
        else if(class == dBOOLEAN)
                printf("%s",*((BOOLEAN*)self) ? "TRUE" : "FALSE");
        else
                printf("[%p]",(void*)self);
        return(parameters[0]);
}

OBJPTR new(parameters)
        OBJPTR *parameters[];
        /* self (parameters[0]) is a class definition object.
         * Create a new instance of that class with space for
         * its instance variables.
         */
{
        OBJPTR *class_def;
        OBJPTR *varlist;
        OBJPTR class_symbol;
        int nvars;

        class_def = (OBJPTR *)parameters[0];

        /* Get instance variable list from class_def[1] */
        varlist = (OBJPTR *)class_def[1];

        /* Get count of instance variables (varlist[1] = count) */
        if(varlist)
                nvars = (int)(long)varlist[1];
        else
                nvars = 0;

        /* Look up the class symbol by name so ClassSymbol() and
         * GetClass() work correctly on the new instance.
         * class_def[5] is the class name string (set in MkClass).
         */
        class_symbol = lookup(table, (char *)class_def[5]);

        /* Create new instance: nvars pointers worth of storage */
        return(NewInstance(class_symbol, nvars * POINTERSIZE));
}

OBJPTR assign(parameters)
        OBJPTR *parameters[];
{
        OBJPTR *address;

        address = parameters[0][0];
        *address = parameters[1];
        return(parameters[1]);
}

OBJPTR sysdump(parameters)
        OBJPTR *parameters[];
{
        BOOLEAN *ptr;
        dump(table);
        ptr = (BOOLEAN *)NewInstance(dBOOLEAN,POINTERSIZE);
        ptr[0] = 1;
        return((OBJPTR)ptr);
}
