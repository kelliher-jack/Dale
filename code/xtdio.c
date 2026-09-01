#include "stdio.h"
#include "ctype.h"

char *getarg(n,s,size,argc,argv)

  int  n;
  char *s;
  int  size;
  int  argc;
  char *argv[];

{
	char *str;
	int i;

  if (n < 0 | n >= argc) return(NULL);
  i=0;
  str=argv[n];
  while (i<size) {
      if (str[i]==NULL) break;
      s[i]=str[i];
      i++;
      }
  s[i]=NULL;
  return(s);
}


normalize(fname,fext)
  char *fname,*fext;

{

  int i;
  char c;

  /* to upper case */
  i=0;
  while(c=fname[i])
  	 if (c == '\n' || c == '\r') {
      	fname[i] = '\0';
        break;
  	 }
     else
         fname[i++]=toupper(c);

	if (--i < 0) return(0); /* null fname */

  /* if no '.', add '.'fext */

  if(find(fname,i+1,'.') < 0) {
    fname[i+1]='.';
    strcpy(&fname[i+2],fext);
  }
}

/* find(addr,len,ch) -- find first
 * occurence of ch in addr to
 * addr+len-1, return 0--len or -1 if
 * not found
 */

find(addr,len,ch)
  char *addr,ch;
  int len;

{
	int i;

	for(i=0;i<len && addr[i] != ch; i++) ;
     return(i<len ? i : -1);
}

/* move(a,b,len) -- move len chars from
 * a to b, inc-ing addresses
 * use strncpy
 */

_itoa(n,s)
	char s[];
	int n;
{
	int i,sign;

	if ((sign = n) < 0) n = -n;
	i = 0;
	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}

reverse(s)
	char s[];
{
	int c,i,j;

	for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}
