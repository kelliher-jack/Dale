#include "stdio.h"
#include "dale.h"

mpattern(msgs,index)
int *index;
OBJPTR msgs[];
  {
      /* global char id[] */
    if(!keyword(id)) return(0);
    msgs[(*index)++]=add(table,id);
    if(vpattern(msgs,index)) return(1);
    if(epattern(msgs,index)) return(1);
    else return(1);
  }

ppattern(msgs,index)
int *index;
OBJPTR msgs[];
  {
    if(!match("self-msg")) return(0);
    if(vpattern(msgs,index)) return(1);
    if(epattern(msgs,index)) return(1);
    else return(1);
  }

evalarg(msgs,index)
int *index;
OBJPTR msgs[];
  {
      /* global char id[] */
    if(!match("$")) return(0);
    else {
      if(identifier(id)) {
        if(*index<32) {
          msgs[(*index)++]=add(table,id);
          msgs[(*index)++]=(OBJPTR)1;
          msgs[(*index)++]=add(table,"object");
        }
        else e5(); /* message pattern too large */
        return(1);
      }
      else {
        e1(); /* illegal identifier */
        return(1);
      }
    }
  }

valarg(msgs,index)
int *index;
OBJPTR msgs[];
  {
      /* global char id[] */
    if(!match("&")) return(0);
    else {
      if(identifier(id)) {
        if(*index<32) {
          msgs[(*index)++]=add(table,id);
          msgs[(*index)++]=(OBJPTR)2;
          msgs[(*index)++]=add(table,"string");
        }
        else e5(); /* message pattern too large */
        return(1);
      }
      else  {
        e1(); /* illegal identifier */
        return(1);
      }
    }
  }

epattern(msgs,index)
int *index;
OBJPTR msgs[];
  {
      /* global char id[] */
    if(!evalarg(msgs,index)) return(0);
    if(match("$")) {
      if(!keyword(id)) {
        e7(); /* keyword expected */
        return(1);
      }
      if(*index < 32)
        msgs[(*index)++]=add(table,id);
      else e5(); /* message pattern too large */
      return(1);
    }
    else if(keyword(id)) {
      if(*index < 32)
        msgs[(*index)++]=add(table,id);
      else e5(); /* message pattern too large */
      if(vpattern(msgs,index)) return(1);
      else if(epattern(msgs,index)) return(1);
      else {
        error("value or eval pattern expected");
        kill();
        return(1);
      }
    }
    else return(1);
  }

vpattern(msgs,index)
int *index;
OBJPTR msgs[];
  {
      /* char id[] */
    if(!valarg(msgs,index)) return(0);
    if(!match("&")) {
      error("& expected");
    }
    if(!keyword(id)) {
      e7(); /* keyword expected */
      return(1);
    }
    if(*index < 32)
      msgs[(*index)++]=add(table,id);
    else e5(); /* message pattern too large */
    if(vpattern(msgs,index)) return(1);
    else if(epattern(msgs,index)) return(1);
    else return(1);
  }

primitive(ptr)
	OBJPTR *ptr;
  {
    char token;
    int  addr;

    if(!match("primitive")) return(0);
    if(match("call")) {
      number(&addr);
      *ptr = MkCall(addr);
      return(TRUE);
    }
    if(match("code"))
      while(1) {
        scan(&token);
        if(token == ';') { MkCode(); return(TRUE); }
      }
    else {
      error("CALL or CODE expected");
      kill();
      return(1);
    }
  }
