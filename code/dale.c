#include "stdio.h"
#include "dale.h"
#include "hash.h"
 
  char dip1[dipsize],dip2[dipsize],quote;
  int  dip1ptr,dip2ptr;
  char line[linesize];
  char mline[linesize];
  char id[linesize];
  int  OptionL;  /* command line options */
  int  lptr,mptr;
  int  errcnt,
           eof,
       cmode;

  FILE  *inp,    /* source input file id   */
        *outp, /* output file id         */
        *inp2;   /* aux file id (includes) */
    
  OBJPTR *currentclass;
  OBJPTR *currentmsg;
  OBJPTR *currentobject;  /* current object in eval print loop */

  int domain;

  char *dCLASS,*dMETHODS,*dSTRING,*dFUNCTION,*dBLOCK,*dCODE,*dDALE;
  char *dVARIABLES,*dMSG,*dEXPR,*dRETURN,*dASIGN,*dINTEGER,*dSELF;
  char *dLABEL,*dGOTO,*dVAR,*dPARAMETER,*dPRIM,*dSYMBOL,*dINSTANCE;
  char *dEXPRLIST,*dBOOLEAN,*dADDR;

  OBJPTR iadd(),isub(),imult(),idiv(),imod(),iequ(),inotequ(),iless();
  OBJPTR ilessequ(),igreater(),inotless(),iincr(),idecr(),iprint();
  OBJPTR ilprint(),bprint(),slprint(),oprint(),assign(),sysdump();
  OBJPTR (*procedure[])() = {
        iadd,isub,imult,idiv,imod,iequ,inotequ,iless,
        ilessequ,igreater,inotless,iincr,idecr,iprint,
        ilprint,bprint,slprint,oprint,assign,sysdump
  };

  OBJPTR table[tblsize];
  

/*
 * Top level of DALE interpreter
 * (C) 1983 Jack S. Kelliher
 */

main(argc,argv)
     int argc;
     char *argv[];

{

  int i;
  char s[120],t[120];  
  FILE *fopen();

  init_table(table,tblsize);

  /* initialization of predefined diphthongs (,),;,($ and } */
  dip1[0]='(';
  dip1[1]=')';
  dip1[2]=';';
  dip1[3]='&';
  dip1[4]='$';
  dip1ptr=5;
  dip2[0]='(';
  dip2[1]='$';
  dip2[2]='$';
  dip2[3]=')';
  dip2[4]='<';
  dip2[5]='-';
  dip2ptr=6;
  quote='"';

  dCLASS=add(table,"_class");
  dMETHODS=add(table,"_methods");
  dSTRING=add(table,"string");
  dFUNCTION=add(table,"_function");
  dCODE=add(table,"_code");
  dDALE=add(table,"_dale");
  dBLOCK=add(table,"_block");
  dVARIABLES=add(table,"_variables");
  dMSG=add(table,"_msg");
  dEXPR=add(table,"_expr");
  dEXPRLIST=add(table,"_exprlist");   
  dRETURN=add(table,"_return");
  dASIGN=add(table,"_asign");
  dINTEGER=add(table,"integer");
  dSELF=add(table,"self");
  dLABEL=add(table,"_label");
  dGOTO=add(table,"_goto");
  dVAR=add(table,"_var");
  dPARAMETER=add(table,"_parameter");
  dPRIM=add(table,"_primitive");
  dSYMBOL=add(table,"_symbol");
  dINSTANCE=add(table,"_instance");
  dBOOLEAN=add(table,"boolean");
  dADDR=add(table,"_address");

  inp=inp2=NULL;
  currentobject=NULL;

  lptr=mptr=errcnt=eof=cmode=0;
  OptionL=0;
      
  fputs("\f",stderr);
  fputs("                                   DALE\n",stdout);
  fputs("                    Copyright (C) 1984 Jack S. Kelliher\n",stdout);
  fputs("                           All Rights Reserved\n\n",stdout);

  for (i=1;i<argc;i++) {
    getarg(i,s,100,argc,argv);
    /* normalize(s,"D"); */
    strcpy(t,s);
    if ((inp=fopen(s,"r"))==NULL) {
      fprintf(stdout,"  can't open file %s\n",s);
      exit(1);
    }
    s[find(s,100,'.')]=0;
    normalize(s,"IMM");
/*      if ((outp=fopen(s,"w"))==NULL) {
                putc(EOL,stdout);
                fputs(" can't open ",stdout);
                fputs(s,stdout);
                exit(1);
        }
*/
    printf("\n  %s -> %s\n\n",t,s);
    parse();
    putchar(EOL);
    ersum();

    /* dump the symbol table for debugging — shows all classes,
       their instance variables, methods, and code blocks */
    dump(table);

    if(inp) fclose(inp);
/*      fclose(outp); */
  }
/*      if (inp == NULL) {  */
  if (argc <= 1) {
    inp = stdin;
    parse();
    ersum();
  }
/*      }                   */
  return 0;
}
