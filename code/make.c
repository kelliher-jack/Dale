#include "stdio.h"
#include "dale.h"
#include "hash.h"

/*
 * Make Module
 *  Procedures in this module create dale syntatic objects
 * FILES: make.c
 *
 * EXTERNAL PROCEDURES:
 *  all.
 */

MkFctn()

{
  fputs("MkFctn()\n",stdout);
}

MkMethods(list,size)
int size;
OBJPTR list[];

{
    int i,j;
    OBJPTR *ptr;

  ptr=NewInstance(dMETHODS,(size+1)*POINTERSIZE); 
  ptr[0]=(OBJPTR)(size>>1); /* divide by 2 */
  for(i=1,j=0;j<size;i++,j++)
    ptr[i] =list[j];
  currentclass[domain]=ptr;
}

MkClass(id)
  char id[];

{
    int i;
    OBJPTR *pointer;
  pointer=add(table,id);
  if(pointer[OBJPR]) {
    fputs(pointer[STRPTR],stdout);
    fputs(" has been redefined\n",stdout);
    /* free old storage */
  }
  else pointer[OBJPR]=NewInstance(dCLASS,6*POINTERSIZE);
  currentclass=pointer[OBJPR];
  for(i=0;i<5;i++)
    currentclass[i]=NULL;
  currentclass[5]=pointer[STRPTR];
}

AddSuper(id)
  char id[];

{
  currentclass[0]=add(table,id);
}

OBJPTR MkInstance(list,size,searchptr)
        int size;
        OBJPTR list[],searchptr;

{
    int i,j;
    OBJPTR *ptr,*tmp;

  printf("MkInstance(0x%lx,%d,0x%lx)\n",list,size,searchptr); fflush(stdout);
  ptr=NewInstance(dVARIABLES,(size+2)*POINTERSIZE);
  ptr[0]=searchptr;
  ptr[1]=(OBJPTR)(size/3);
  for(i=2,j=0;j<size;) {
    tmp=list[j++];
    ptr[i++]=tmp[STRPTR];
    tmp=list[j++];
    ptr[i++]=tmp[STRPTR];
    ptr[i++]=list[j++];
  }
  return(ptr);
}

OBJPTR MkBlock(vptr,list,size)
int size;
OBJPTR *vptr,list[];

{
    int i,j;
    OBJPTR *blk;
  blk=NewInstance(dBLOCK,(size+2)*POINTERSIZE);
  blk[0]=(OBJPTR)size;
  blk[1]=vptr;
  for(i=2,j=0;j<size;i++,j++)
    blk[i]=list[j];
  return(blk);
}

MkMsg(list,size,methodlist,index)
int size,index;
OBJPTR list[],methodlist[];

{
    int i;
    OBJPTR *ptr,*tmp;
  ptr=NewInstance(dMSG,(size+1)*POINTERSIZE);
  ptr[size]=NULL;
  for(i=0;i<size;i++) {
    tmp=list[i];
    ptr[i]=(tmp<(OBJPTR)4)?(OBJPTR)tmp:tmp[STRPTR];
  }
  methodlist[index]=ptr;
}

MkProc(blockptr,methodlist,index)
int index;
OBJPTR *blockptr,methodlist[];
{
  if (methodlist != NULL)
    methodlist[index]=blockptr;
}

OBJPTR MkExpr(list,size)
int size;
OBJPTR list[];
{
    int i;
    OBJPTR *ptr;
  ptr=NewInstance(dEXPR,(size+1)*POINTERSIZE); /* size for pointers */
  for(i=0;i<size;i++)
    ptr[i]=list[i];
  ptr[size]=NULL;
  return(ptr);
}

OBJPTR MkNum(num)
int num;
{
    OBJPTR *ptr;
  ptr=NewInstance(dINTEGER,POINTERSIZE);
  ptr[0]=(OBJPTR)num;
  return(ptr);
}

OBJPTR MkLabel(id,class,exprnum)
char *id;
OBJPTR *class,exprnum;
{
    OBJPTR *ptr,*str;
  ptr=NewInstance(class,2*POINTERSIZE);
  str=add(table,id);
  ptr[0]=str[STRPTR];
  ptr[1]=exprnum;
  return(ptr);
}

OBJPTR MkVar(str,offset,class)
char str[];
int  offset;
OBJPTR *class;
{
    OBJPTR *ptr;
  ptr=NewInstance(class,2*POINTERSIZE);
  ptr[0]=str;
  ptr[1]=(OBJPTR)offset;
  return(ptr);
}

OBJPTR blockvar(id,searchptr)
  char   *id;
  OBJPTR searchptr;
{
  int i;
  OBJPTR *ptr,*str,*ptr2;
  OBJPTR *sp;   /* properly typed alias for searchptr */

  ptr=NULL;
  ptr2=NULL;
  str=add(table,id);
  str=str[STRPTR];
  sp=(OBJPTR*)searchptr;
  while(sp) {
    for(i=2;i<3*(unsigned int)sp[1];i+=3)
      if(sp[i]==str) {
        ptr=MkVar(str,sp[i+2],dVAR);
        break;
      }
    if(ptr) break;
    sp=(OBJPTR*)sp[0];
  }
  for(i=0;currentmsg[i];i++)
    if(currentmsg[i]==str)
      if((i-1)%4)
        continue;
      else {
        ptr2=MkVar(str,i,dPARAMETER);
        break;
      }
  if((ptr != 0) && (ptr2 != 0))
    error("name conflict");
  else if(ptr2) ptr=ptr2;
  return(ptr);
}
  
OBJPTR MkString(str,size)
char str[];
int  size;
{
    char *ptr;
    int  i;
  ptr=NewInstance(dSTRING,size+1);
  for(i=0;i<size;i++)
    ptr[i]=str[i];
  ptr[size]=NULL;
  return(ptr);
}

OBJPTR MkCall(index)
        int index;  /* index into array of predefined primitive */
                                /* function pointers */

{
        OBJPTR * ptr;

        ptr=NewInstance(dPRIM,POINTERSIZE);
        ptr[0]=(OBJPTR)index;
        return(ptr);
}

MkCode()

{
  fputs("MkCode()\n",stdout);
}
