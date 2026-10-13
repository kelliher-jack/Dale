#include "stdio.h"
#include "dale.h"

/*
 * Parser Module
 *
 * FILES: dale1.c dale2.c dale3.c dale4.c
 *
 * EXTERNAL PROCEDURES:
 *   parse()
 *
 */

/*
 *  Process all input text
 *
 * defines, includes, and asm. program
 * definitions are legal....
 */

parse()

 {

   int i;
   OBJPTR *param[MAXPARAMS],*locs[MAXLOCALS];  /* args for eval */

   eof=0; 
   lptr=0;
   line[0]=0;
   mptr=0;
   errcnt=0;
   inp2=0;
   
   /* do until no more input */
   while (eof==0) {
     if(match("#dipthong")) {
       adddip();
       currentobject=NULL;
     } else { dalestatement(); }
     if(currentobject) {
       if(ClassSymbol(currentobject)==dCLASS) {
         printf("dCLASS: %s.\n",currentobject[5]); fflush(stdout);
       } else {
         eval(currentobject,param,locs);
       }
     }
     /* force eof if pending */
     blanks();
   }
 }

adddip()

{
  blanks();
  if(ch==0)
    error("missing diphthong");
  else if(an(ch))
    {
      error("dipthong can not be alphabetic or mumeric");
      kill();
    }
  else if(nch==0|nch==' ')
    if(dip1ptr<dipsize)
      dip1[dip1ptr++]=gch;
    else
      {
        error("too many single dipthongs");
        kill();
      }
  else if(dip2ptr>=dipsize)
    {
      error("too many double dipthongs");
      kill();
    }
  else
    {
      dip2[dip2ptr++]=gch;
      dip2[dip2ptr++]=gch;
      if(ch!=0 && ch!=' ')
        {
          error("max diphthong length is two");
          kill();
        }
    }
}

kill()
{
  lptr=0;
  line[lptr]=0;
}

/*
 * Report errors for user
 */

ersum()

 { 
  if(errcnt==0)
   {
    fputs("No errors.",stdout);
    putc(EOL,stdout);
   }
  else
   {
      char num[10];

    fputs(" *** ",stdout);
    if(errcnt < 10)
      putc(errcnt+'0',stdout);
    fputs(" ERRORS FOUND ***",stdout);
    putc(EOL,stdout);
   }
 }

error(ptr)
 char ptr[];
 {
  int k;

  putchar(EOL);
  fputs(line,stdout);putc(EOL,stdout);
  k=0;
  while(k<lptr)
    { if(line[k]==tabchar)putc(tabchar,stdout);
      else putc(' ',stdout);
    ++k;
    }
  putc('^',stdout);
  putc(EOL,stdout);
  fputs(ptr,stdout);
  putc(EOL,stdout);
  /* -- Beep bell if first error -- */
  if(errcnt++);
  else putc('\007',stdout);
 }

e1()
  {
    error("illegal identifier");
    kill();
  }

e2()
  {
    error("missing variable name");
  }

e3()
  {
    error("expression expected");
    kill();
  }

e4()
  {
    error("missing ;");
  }

e5()
  {
    error("message pattern too large");
  }

e6()
  {
    error("label expected");
    kill();
  }

e7()
  {
    error("keyword expected");
    kill();
  }
