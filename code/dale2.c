#include "stdio.h"
#include "dale.h"

int dalestatement()

  {
    OBJPTR ptr;
    int offset;

    offset = 0;
    if (function()) { return(TRUE); }
    else if (class()) {
      printf("CLASS\n"); fflush(stdout);      
      if(errcnt) { currentobject=NULL; } else { currentobject=currentclass; }
      return(TRUE);
    } else {
      printf("call exprression\n"); fflush(stdout);      
      if ((ptr=expression(0,&offset,0))) {
        if(!match(";")) {
          e4(); /* missing ; */
        }
        if(errcnt) {
          currentobject=NULL;
          errcnt=0;
        } else { currentobject=(OBJPTR *)ptr; }
        return(TRUE);
      } else {
        error("not a dale statement");
        kill();
        return(FALSE);
      }
    }
  }

int function()

  {
      /* global char id[120] */
      OBJPTR buf[4];
      int  size;

    size=2;
    if (match("function")) {
      if (!identifier(id)) {
        e1(); /* illegal identifier */
        return(TRUE);
      }
      MkFctn();
      if (!match("method:")) {
        error("METHOD: required");
        kill();
        return(TRUE);
      }
      if (!fmethod())
        return(TRUE);
      MkMethods(buf,size);
      if (!match("endclass.")) {
        error("ENDCLASS. required");
        kill();
        return(TRUE);
      }
    }
    else return(FALSE);
    return(FALSE);
  }

int class()

  {
      /* glogal char id[15] */
      OBJPTR buf[48];
      int  size;
      int  prevdomain,offset;

    offset=0;
    if (match("class")) {
      if(!identifier(id)) {
        e1(); /* illegal identifier */
      }
      MkClass(id);
      printf("identifier %s char %d  lptr %d\n",id,(int)line[lptr],lptr);
      fflush(stdout);         
      if(match("superclass:")) {
        if(!identifier(id)) {
          e1(); /* illegal identifier */
        }
        AddSuper(id);
      }
      if(match("instance variables:")) {
        size=0;
        if(!varlist(buf,48,&size,&offset))
          e2(); /* missing variable name */
        if(!match(".")) {
          error("missing .");
          kill();
        }
        currentclass[1]=MkInstance(buf,size,NULL);
      }
      size=0;
      if(match("class methods:")) {
        domain=XCLASS;
        while(methods(buf,48,&size));
        MkMethods(buf,size);
        size=0;
      }
      if(match("instance methods:")==0) {
        error("INSTANCE METHODS: required");
        kill();
      }
      domain=EXTERNAL;
      domainswitch();
      while(methods(buf,48,&size));
      MkMethods(buf,size);
      size=0;
      prevdomain=domain;
      if(domainswitch()) {
        if(domain==prevdomain) 
          error("domain has been redefined");
        while(methods(buf,48,&size));
        MkMethods(buf,size);
      }
      if(!match("endclass.")) {
        error("missing ENDCLASS.");
        kill();
      }
      return(TRUE);
    }
    else return(FALSE);
  }

OBJPTR expression(searchptr,offset,snum)
     OBJPTR searchptr;
     void *offset;
     int snum;
  {
    /* global char id[120] */
    int  exprptr;
    OBJPTR exprlist[48],ptr;

    printf("expression(0x%lx,0x%lx,%d)\n",searchptr,offset,snum); fflush(stdout);
    exprptr=0;
    if(match("label:")) {
      if(!identifier(id)) {
        e6(); /* label expected */
        return(MkExpr(exprlist,exprptr));
      }
      exprlist[exprptr++]=MkLabel(id,dLABEL,snum);
      return(MkExpr(exprlist,exprptr));
    }
    if(match("goto")) {
      if(!identifier(id)) {
        e6(); /* label expected */
        return(MkExpr(exprlist,exprptr));
      }
      exprlist[exprptr++]=MkLabel(id,dGOTO,NULL);
      return(MkExpr(exprlist,exprptr));
    }
    if(identifier(id)) {
      if((ptr=blockvar(id,searchptr))) {
        exprlist[exprptr++]=ptr;
      } else {
        exprlist[exprptr++]=add(table,id);
      }
      if(match("<-")) {
        exprlist[exprptr++]=dASIGN;
        if((ptr=expression(searchptr,offset,snum))==NULL) {
          e3(); /* expression expected */
          return(MkExpr(exprlist,exprptr));
        }
        exprlist[exprptr++]=ptr;
      } else if(message(exprlist,&exprptr,searchptr,offset)) {
        if(match("<-")) {
          exprlist[exprptr++]=dASIGN;
          if((ptr=expression(searchptr,offset,snum))==NULL) {
            e3(); /* expression expected */
            return(MkExpr(exprlist,exprptr));
          }
          exprlist[exprptr++]=ptr;
        }
      } else {
        parameter(exprlist,&exprptr,searchptr,offset);
      }
      return(MkExpr(exprlist,exprptr));
    }
    if(primary(exprlist,&exprptr,searchptr,offset)) {
      message(exprlist,&exprptr,searchptr,offset);
      return(MkExpr(exprlist,exprptr));
    }
    else {
      return(NULL);
    }
  }
