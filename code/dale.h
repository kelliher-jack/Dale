#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif
#define BOOLEAN int

#ifdef ecoc
#define _ctype_  _ctype
#define malloc(x) calloc(1,x)
extern char *calloc();
#define _U  0x02
#define _L  0x01
#define _N  0x04
#else
/* _ctype_ character class table — indexed by (char)+1.
 * Bits: _U=uppercase, _L=lowercase, _N=numeric, _S=space,
 *       _P=punctuation, _C=control, _X=hex digit */
#define _U  0x02
#define _L  0x01
#define _N  0x04
#define _S  0x08
#define _P  0x10
#define _C  0x20
#define _X  0x40

extern unsigned char _ctype_[];
extern void *malloc(size_t);
#endif

#define EOL '\n'

#define INTERNAL 4
#define EXTERNAL 3
#define XCLASS   2
#define tabchar '\t'
#define namesize 13
#define namemax  12
#define linesize 200
#define linemax linesize-1
#define mpmax linemax
#define dipsize 30
#define tblsize 128

#define ch line[lptr]
#define nch (line[lptr] ? line[lptr+1] : 0)
#define gch (line[lptr] ? line[lptr++] : 0)
#define alpha(c) ((_ctype_[1+(c)]&(_U|_L))|((c)=='_'))
#define numeric(c) (_ctype_[1+(c)]&(_N))
#define an(c) ((_ctype_[1+(c)]&(_U|_L|_N))|((c)=='_'))

typedef char *OBJPTR;

  /* size in bytes of a pointer (machine dependent) */
#define POINTERSIZE sizeof(OBJPTR)

extern OBJPTR *currentclass;
extern OBJPTR *currentmsg;
extern OBJPTR *currentobject; /* current object in the eval print loop */

extern int  domain;

extern char *dCLASS,*dMETHODS,*dSTRING,*dRETURN,*dLABEL,*dPARAMETER,
            *dFUNCTION,*dCODE,*dBLOCK,*dASIGN,*dSELF,*dGOTO,*dPRIM,
            *dDALE,*dVARIABLES,*dMSG,*dEXPR,*dINTEGER,*dBOOLEAN,*dVAR,
            *dSYMBOL,*dINSTANCE,*dEXPRLIST,*dADDR;

extern OBJPTR iincr(),ilprint(),slprint();
extern OBJPTR (*procedure[])();

extern int OptionL;  /* command line options */

extern OBJPTR table[tblsize];
extern char dip1[dipsize];
extern char dip2[dipsize];
extern int  dip1ptr,dip2ptr;
extern char quote;
extern char line[linesize];
extern char mline[linesize];
extern char id[linesize];
extern int  lptr,mptr;
extern int  errcnt, eof, cmode;
extern FILE *inp,*outp,*inp2;

/* external functions */
extern char *getarg();
extern char *strcpy(char *, const char *);
extern void normalize(char *, char *);
extern int find(char *, int, char);
extern void init_table(OBJPTR *table, int size);
extern int streq(char *, char *);

extern OBJPTR scan();
extern OBJPTR mkname();
extern OBJPTR add();
extern OBJPTR lookup();
extern int match();
extern int identifier(char id[]);
extern int keyword();
extern int number();
extern void blanks();
extern int inbyte();
extern int evalarg();
extern int valarg();
extern int vpattern();
extern int epattern();
extern int msgpattern();
extern int ppattern();
extern int primary();
extern int message();
extern int parameter();
extern int block();
extern int vardeclaration();
extern int varlist();
extern int domainswitch();
extern int methods();
extern int mpattern();
extern int argtypelist();
extern int primitive();
extern int MkFctn();
extern int fmethod();
extern int MkClass();
extern int AddSuper();
extern int MkCode();
extern int MkMsg();
extern int MkMethods();
extern int MkProc();
extern OBJPTR NewInstance();
extern OBJPTR blockvar();
extern OBJPTR eval();
extern int parse();
extern int adddip();
extern int dalestatement();
extern int function();
extern int class();
extern OBJPTR expression(OBJPTR sptr, void *offset, int snum);
extern int ersum();
extern int error();
extern int kill();
extern int e1();
extern int e2();
extern int e3();
extern int e4();
extern int e5();
extern int e6();
extern int e7();

extern OBJPTR MkInstance();
extern OBJPTR MkBlock();
extern OBJPTR MkExpr();
extern OBJPTR MkNum();
extern OBJPTR MkLabel();
extern OBJPTR MkVar();
extern OBJPTR blockvar();
extern OBJPTR MkString();
extern OBJPTR MkCall();

extern OBJPTR NewInstance();
extern OBJPTR GetClass();
extern OBJPTR ClassSymbol();

/* end of global file */
