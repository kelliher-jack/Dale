#include "stdio.h"
#include "dale.h"

sexpression(slist,index,searchptr,offset)
int *index,*offset;
OBJPTR searchptr;
OBJPTR slist[];
  {
      OBJPTR ptr;
    while(1) {
      if((ptr=expression(searchptr,offset,*index))==FALSE) return(FALSE);
      slist[(*index)++]=ptr;
      if(*index > 46) {
        *index = 46;
        error("too many statments");
      }
      if(!match(";")) {
        e4(); /* missing ; */
      }
    }
    return(TRUE);
  }

statements(slist,index,searchptr,offset)
int *index,*offset;
OBJPTR searchptr;
OBJPTR slist[];
  {
      OBJPTR *ptr;
    sexpression(slist,index,searchptr,offset);
    if(match("return")) {
      if((ptr=expression(searchptr,offset,*index))==NULL) {
        e3(); /* expression expected */
        return(TRUE);
      }
      slist[(*index)++]=ptr;
      if(!match(";"))
        e4(); /* missing ; */
      return(TRUE);
    }
    ptr=NewInstance(dEXPR,2*POINTERSIZE);
    ptr[0]=dSELF;
    ptr[1]=NULL;
    slist[(*index)++]=ptr;
    return(TRUE);
  }

message(list,idx,searchptr,offset)
int *idx,*offset;
OBJPTR searchptr;
OBJPTR list[];
  {
      /* global char id[] */
      OBJPTR *ptr;
    if(!keyword(id)) return(FALSE);
    if((ptr=blockvar(id,searchptr)))
      list[(*idx)++]=ptr;
    else
      list[(*idx)++]=add(table,id);
    if(message(list,idx,searchptr,offset)) return(TRUE);
    if(parameter(list,idx,searchptr,offset)) return(TRUE);
    return(TRUE);
  }

parameter(list,idx,searchptr,offset)
int *idx,*offset;
OBJPTR searchptr;
OBJPTR list[];
  {
    if(!primary(list,idx,searchptr,offset)) return(FALSE);
    message(list,idx,searchptr,offset);
    return(TRUE);
  }

primary(list,idx,searchptr,offset)
int *idx,*offset;
OBJPTR searchptr;
OBJPTR list[];
  {
      int num,i;
          OBJPTR *ptr;
      /* global char id[] */
    if(identifier(id)) {
      if((ptr=blockvar(id,searchptr)))
        list[(*idx)++]=ptr;
      else
        list[(*idx)++]=add(table,id);
      return(TRUE);
    }
    else if(number(&num)) {
      list[(*idx)++]=MkNum(num);
      return(TRUE);
    }
    else if(block(&ptr,searchptr,offset)) {
      list[(*idx)++]=ptr;
      return(TRUE);
    }
    else if(match("(")) {
      if((ptr=expression(searchptr,offset,*idx))==FALSE) {
        e3();  /* expression expected */
        return(TRUE);
      }
      list[(*idx)++]=ptr;
      if(!match(")")) {
        error("closing parin expected");
        return(TRUE);
      }
      return(TRUE);
    }
    else if(ch==quote) {
      gch;
      i=0;
      while(ch!=quote)
        id[i++]=gch;
      gch;
      id[i]=NULL;
      list[(*idx)++]=MkString(id,i);
    }
    else
      return(FALSE);
  }

/* blockstruct [0]=number of statements            */
/*             [1]=pointer to variable list        */
/*             [2...n]=pointers to statement lists */

block(ptr,searchptr,offset)
    int *offset;
    OBJPTR searchptr;
    OBJPTR *ptr;
  {
      int size;
      OBJPTR list[48],vptr;

    printf("block(0x%lx, 0x%lx, 0x%lx)\n",ptr,searchptr,offset); fflush(stdout);          
    size=0;
    if(!match("($")) return(FALSE);
    vardeclaration(list,&size,offset);
    vptr=MkInstance(list,size,searchptr);
    printf("MkInstance returned 0x%lx searchptr\n",vptr); fflush(stdout);    
    size=0;
    statements(list,&size,vptr,offset);
    if(!match("$)"))
      error("$) expected");
    *ptr=MkBlock(vptr,list,size);
    return(TRUE);
  }

vardeclaration(list,size,offset)
int *size,*offset;
OBJPTR list[];
  {
    *size=0;
    if(!match("var")) return(FALSE);
    if(!varlist(list,32,size,offset))
      e2(); /* missing variable name */
    if(!match("."))
      error("missing period");
    return(TRUE);
  }

varlist(list,max,index,offset)
int max,*index;
OBJPTR list[];
int *offset;
  {
      /* global char id[] */

    if(!identifier(id)) return(FALSE);
    list[*index]=add(table,id);
    if(identifier(id)) list[*index+1]=add(table,id);
    else list[*index+1]=add(table,"object");
    list[*index+2] = (OBJPTR)*offset;
    (*offset)++;
    if(*index<max-3) *index = *index + 3;
    else error("too many variables");
    if(!match(";")) return(TRUE);
    if(!varlist(list,max,index,offset)) {
      e2(); /* missing variable name */
      return(TRUE);
    }
    return(TRUE);
  }

domainswitch()
  {
    if(match("external")) {
      domain=EXTERNAL;
      return(TRUE);
    }
    else if(match("internal")) {
      domain=INTERNAL;
      return(TRUE);
    }
    else return(FALSE);
  }

methods(list,max,index)
  int max,*index;
  OBJPTR list[];

  {
      int size,offset;
      OBJPTR blockptr;
      OBJPTR msgs[32];

      
    size=0;
    offset=0;
    if(!mpattern(msgs,&size)) return(0);
    MkMsg(msgs,size,list,*index);
    printf("msgs size %d index %d\n",size, *index);
    fflush(stdout);
    currentmsg=list[*index];
    argtypelist();
    *index = *index+2;
    if(*index >= max) {
      error("too many methods");
      (*index)-=2;
    }
    if(block(&blockptr,NULL,&offset)) MkProc(blockptr,list,*index-1);
    else if(primitive(&blockptr)) MkProc(blockptr,list,*index-1);
    return(1);
  }

fmethod()
  {
      int index,offset;
      OBJPTR blockptr;
      OBJPTR msgs[4];
    offset=0;
    if(!ppattern(msgs,&index)) return(0);
    MkMsg(msgs,index,msgs,index); /*fix later*/
    argtypelist();
    if(block(&blockptr,NULL,&offset)) { MkProc(NULL,NULL,NULL); return(1); }
    if(primitive()) { MkProc(NULL,NULL,NULL); return(1); }
    return(1);
  }


argtypelist()
  {
      int size;
      int offset;
      OBJPTR list[64];
    offset=0;
    return(varlist(list,64,&size,&offset));
  }
