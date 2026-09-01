#include "stdio.h"
#include "dale.h"
#include "hash.h"

/*  tablesize  size of hash table (number of buckets) is stored
    in table[0].  */

/*
 * Hash(s) returns an index into a buckette array of tablesize size.
 */

hash (s,tablesize)
int    tablesize;
char   *s;

{
    int     hashval;
    int     magic;

    magic = 59;
    tablesize=tablesize-1;
    for (hashval = 0; *s != NULL;)
        hashval = (hashval + (*s++) * magic) % tablesize;
    return (hashval+1);
}


/*
 * scan(head,id,ptr) returns a pointer to the bucket. 
 * ptr will point to the prievious bucket in the chain.
 */

OBJPTR scan(head,id,ptr)
	OBJPTR  *head,*ptr;
	char id[];

{
    OBJPTR *current;

    current = *head;
    *ptr=head;
    
    while(1) {
      if (current==NULL) return(NULL);
      else if (strcmp(current[STRPTR],id)==0) return(current);
      else {
        *ptr=current;
        current=current[LNKPTR];
      }
    }
}


/*
 * insert an item to the end of a list after position
 * 'hear' which must be the current end of list.
 */


insert(name,item,hear)
OBJPTR *item, hear[];
char name[];

{
    OBJPTR *link;

  if((link = NewInstance(dSYMBOL,3*POINTERSIZE))) {
    link[LNKPTR]=NULL;
    link[STRPTR]=name;
    link[OBJPR]=item;
    hear[LNKPTR]=link;
    }
  else memfault();
}

/*
 * Mkname(id) creates as string for id and returns the address of it.
 */

OBJPTR mkname(id)
char id[];

{
    char *temp;

  if(temp=(OBJPTR)NewInstance(dSTRING,strlen(id)+1)) {
    strcpy(temp,id);
    return(temp);
  }
  else
    memfault();
}

memfault()
{
  fputs("overflow of hashtable - unable to continue.\n",stderr);
  exit(1);
}

init_table(table,size)
int size;
OBJPTR table[];
            
{
    int i,tablesize;

    table[SIZE]=(OBJPTR)(tablesize=size);
    for(i=1;i<tablesize;i++) table[i]=NULL;
}
