#include "stdio.h"
#include "dale.h"

/*
 * Lex Module
 *
 * FILES: lex.c
 *
 * EXTERNAL PROCEDURES:
 *  boolean number(val)      int val[];
 *  boolean identifier(id)   char id[];
 *  boolean keyword(id)      char id[];
 *
 */

int reserved(char *p);

int number(val)
  int val[];
{
  int k,minus,base;
  char c;
  k=minus=1;
  base=10;  /* start with base 10 */
  while(k){
    k=0;
    if(match("+"))k=1;
    if(match("-")){
      minus=(-minus);
      k=1;
    }
  }
  if(numeric(ch)==0)return 0;

  if(ch=='0'){
    gch; /* gobble 0 */
    c=ch; /* examine next char */
    if((c=='x') | (c=='X')){
      base=16;
      gch; /* gobble "x" */
    }
    else base=8;
  }

  while(1){
    c=ch;
    if(numeric(c))
      k=k*base+(c-'0');
    else if(base==16){
      if((c>='A') & (c <='F'))c=c-55;
      else if((c>='a') & (c <='f'))c=c-87;
      else break; /* not hex */
      k=k*base+c;
    }
    else break; /* not diget */
    inbyte(); /* gobble char */
  }
  if (minus<0) k=(-k);
  val[0]=k;
  return 1;
}

int identifier(id)

  char id[];

{
    char *p;
    int  i;

  i=0;
  blanks();
  if(!alpha(ch)) return(FALSE);
  p=line+lptr;
  if (reserved(p))
    return(FALSE);
  id[i++]=gch;
  while((ch=='-')|an(ch))
    id[i++]=gch;
  id[i]='\0';
  return(TRUE);
}

int keyword(id)

  char id[];

{

    char *p;
    int  i;

  i=0;
  blanks();
  if((ch==';')|(ch=='(')|(ch==')')|(ch==quote)|numeric(ch)|!ch) {
    return(FALSE);
  }
  p=line+lptr;
  if(reserved(p))
    return(FALSE);
  id[i++]=inbyte();
  while((ch!=' ') && (ch!=0))
    id[i++]=inbyte();
  id[i]='\0';
  return(TRUE);
}

int reserved(p)
char *p;
{
  if(streq(p,"return") | streq(p,"var") | streq(p,"instance") |
      streq(p,"endclass") | streq(p,"external") | streq(p,"primitive") |
      streq(p,"internal") | streq(p,"goto") | streq(p,"label"))
    return(TRUE);
  else return(FALSE);
}

