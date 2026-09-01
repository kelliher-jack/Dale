#include "stdio.h"
#include "dale.h"
#include "hash.h"

void printexpr(int ptr,OBJPTR *space);

void dump(table)
OBJPTR table[];

{
    int  i,tablesize;
    OBJPTR  *class,*current,*ptr,ClassSymbol();

    tablesize=(int)table[SIZE];
    printf("dump(0x%lx)  size %d\n",table,tablesize); fflush(stdout);    
    for(i=1;i<tablesize;i++) {
      if (table[i])
	printf("table[%d] 0x%lx\n",i,table[i]);
        for(current=(OBJPTR *)table[i]; current; current=current[LNKPTR]) {
          fputs(current[STRPTR],stdout);
          class=NULL;
          if((ptr=current[OBJPR])) {
            class=ClassSymbol(ptr);
            fputs(" - ",stdout);
            fputs(class[STRPTR],stdout);
          }
          putchar(EOL);
          if (class==dCLASS) printclass(ptr);
        }
    }
}

void printclass(ptr)
OBJPTR *ptr;
{
    int j,k,l;
    OBJPTR *s,*tmp;

  s=ptr[0];
  if(s) {
    fputs("  superclass: ",stdout);
    fputs(s[STRPTR],stdout);
    putchar(EOL);
  }
  tmp=ptr[1];
  if (tmp)
    printvar(tmp,2);
  for(j=2;j<5;j++)
    if(ptr[j]) {
      s=ptr[j]; /* s=ptr to method */
      fputs("  METHODS ",stdout);
      putchar(j+'0');
      putchar(EOL);
      for(k=1;k<(int)s[0]*2+1;k+=2) {
        tmp=s[k]; /* tmp=ptr to template */
        tab(4);
        for(l=0;tmp[l];l++)
          if((l-2)%4 == 0) {  /* field is the parameter offset */
            printf("%d ",tmp[l]);
          }
          else {
            fputs(tmp[l],stdout);
            putchar(' ');
          }
        putchar(EOL);
        tmp=s[k+1]; /* tmp=pointer to block */
        if(ClassSymbol(tmp)==dBLOCK) printblock(tmp,4);
        else
        	printf("      PRIMITIVE call[%d]\n",tmp[0]);
      }
    }
}

printvar(tmp,size)
int size;
OBJPTR *tmp;
{
   int  j;
   char num[8];

 tab(size);
 if(tmp) {
   fputs("instance vars: ",stdout);
   _itoa(tmp[1],num);
   fputs(num,stdout);
   putchar(EOL);
   for(j=2;(j<3*(int)tmp[1]) && (int)tmp;) {
     tab(size+2);
     fputs(tmp[j++],stdout);
     putchar(' ');
     fputs(tmp[j++],stdout);
     fputs(" = ",stdout);
     _itoa(tmp[j++],num);
     fputs(num,stdout);
     putchar(EOL);
   }
 }
}


printblock(ptr,space)
int space;
OBJPTR *ptr;
{
  int i;

  tab(space);
  fputs("BLOCK\n",stdout);
  printvar(ptr[1],space+2);
  for(i=2;i<(int)ptr[0]+2;i++)
    printexpr(ptr[i],space+2);
}

printexpr(ptr,space)
int space;
OBJPTR *ptr;

{
  int  i;
  OBJPTR *tmp,*cptr,ClassSymbol();

  i=0;
  tab(space);
  fputs("EXPRESSION\n",stdout);
  while(ptr[i]) {
    cptr=(OBJPTR *)ClassSymbol(ptr[i]);
    if(cptr == (OBJPTR *)dBLOCK)
      printblock(ptr[i],space+2);
    else if (cptr == (OBJPTR *)dEXPR)
      printexpr(ptr[i],space+2);
    else if (cptr == (OBJPTR *)dSTRING) {
      tab(space+2);
      printf("STRING \"%s\"\n",ptr[i]);
    }
    else if (cptr == (OBJPTR *)dINTEGER) {
      tab(space+2);
      printf("NUMBER %d\n",(int)*(ptr[i]));
    }
    else if (cptr == (OBJPTR *)dLABEL)
      printspec("LABEL ",ptr[i],space+2);
    else if (cptr == (OBJPTR *)dGOTO)
      printspec("GOTO ",ptr[i],space+2);
    else if (cptr == (OBJPTR *)dPARAMETER)
      printspec("PARAMETER ",ptr[i],space+2);
    else if (cptr == (OBJPTR *)dVAR)
      printspec(NULL,ptr[i],space+2);
    else {
      tmp=(OBJPTR *)ptr[i];
      tab(space+2);
      fputs(tmp[STRPTR],stdout);
      putchar(EOL);
    }
    i++;
  }
}

printspec(msg,ptr,space)
char *msg;
int  space;
OBJPTR *ptr;
{
  char num[10];

  tab(space);
  if(msg) fputs(msg,stdout);
  fputs(ptr[0],stdout);
  fputs(" = ",stdout);
  _itoa(ptr[1],num);
  fputs(num,stdout);
  putchar(EOL);
}  

tab(space)
int space;
{
    int i;
  for(i=0;i<space;i++)
    putchar(' ');
}
