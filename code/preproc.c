#include "stdio.h"
#include "dale.h"

int inbyte();
int inchar();
void inline();

/*
 * Preprocessor Module
 *
 * FILES: preproc.c
 *
 * EXTERNAL PROCEDURES:
 *  boolean match(lit)		char *lit;
 *  void blanks()
 *  int inbyte()
 *  int inchar()
 *  void inline()
 *
 */

/*
 * keepch(c) stores the charactor <c> in the temporary work space
 * macro-expanding line input buffer <mline>.
 */

int keepch(c)
  char c;
{
  mline[mptr]=c;
  if(mptr<mpmax)mptr++;
  return c;
}

/*
 * preprocess() - preprocesses the current line input buffer.
 * excess leading spaces are striped off and dipthongs are recognized
 * and surrounded by blanks.
 */

void preprocess()

{

    int k,flag;

  mptr=lptr=0;
  while(ch)
    {
      if(ch=='\r') { inchar(); continue; }
      if(an(ch)) keepch(gch);
      else if(ch==' ') keepch(gch);
      else if(ch==quote)
        {
          keepch(gch);
          while(ch!=quote)
            {
              if(ch)
                keepch(gch);
              else
                {
                  error("missing quote");
                  keepch(quote);
                  break;
                }
            }
          keepch(gch);
        }
      else if((ch=='/')&(nch=='*'))
        {
          inchar();
          inchar();
          while((ch!='*') | (nch!='/'))
            {
              inchar();
              if(eof)break;
            }
          inchar();
          inchar();
        }
      else
        {
          flag=TRUE;
          for(k=0;k<dip2ptr;k++)
            if(ch==dip2[k++] && nch==dip2[k])
              {
                keepch(' ');
                keepch(gch);
                keepch(gch);
                keepch(' ');
                flag=FALSE;
                break;
              }
          if(flag)
            {
              flag=TRUE;
              for(k=0;k<dip1ptr;k++)
                if(ch==dip1[k])
                  {
                    keepch(' ');
                    keepch(gch);
                    keepch(' ');
                    flag=FALSE;
                    break;
                  }
               if(flag) keepch(gch);
            }
        }
    }
  keepch(0);
  if(mptr>=mpmax)
    error("line too long");
  lptr=0;
  strcpy(line,mline);
  mptr=strlen(line)+2;
  if (OptionL) {
  	fputs(line,stdout);
	putchar(EOL);
  }
}

int streq(str1,str2)
  char str1[],str2[];
 {
  int k;
  k=0;
  while (str2[k])
    { if((str1[k])!=(str2[k]))return 0;
    k++;
    }
  return k;
 }

int match(lit)
  char *lit;
 {
  int k;

  blanks();
  if ((k=streq(line+lptr,lit)))
    {
      lptr=lptr+k;
      return 1;
    }
   return 0;
}

void blanks()
{

	while(1) {
		while(ch==0) {
			inline();
	        preprocess();
	        if(eof)break;
		}
	    if(ch==' '){(void)gch;}
	    else if(ch==tabchar){(void)gch;}
	    else if(ch=='\r'){(void)gch;}
	    else return;
	}
}

int inbyte()
{
  while(line[lptr]==0)
   {
    if (eof) return 0;
    inline();
    preprocess();
   }
  return gch;
}

int inchar()
{
  if(line[lptr]==0) inline();
  if(eof) return 0;
  return(gch);
}

/*
 * inline() - Precondition before calling: current charactor <ch or gch> in
 * the line input buffer <line> is a 0. i.e. the buffer is empty.
 * 
 * inline fills the line input buffer <line> from the currently active input
 * file.  It fills the buffer until a eol is reached or the buffer is full.
 * If the } of the source file is reached the global var eof is set to
 * 1  i.e. TRUE
 */

void inline()
 {
  int k;
  FILE *unit;

  while(1)
   {
    if(eof) return;
    if((unit=inp2)==NULL) unit=inp;
    kill();
    while((k=getc(unit))!=EOF)
     {
      if((k==EOL)||(lptr>=linemax)) break;
      line[lptr++]=k;
     }
    line[lptr]=0;  /* append null */
    if(k==EOF)
     {
      fclose(unit);
      if(inp2)inp2=NULL;
      else eof=1;
     }
    if(lptr)
     {
      lptr=0;
      return;
     }
   }
 }
