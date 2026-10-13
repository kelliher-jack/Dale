#include "stdio.h"
#include "dale.h"
#include "hash.h"

/* Object Primitive Procedures Module
 *
 * FILES: objprim.c
 *
 * EXTERNAL PROCEDURES:
 *  OBJPTR NewInstance(ClassPtr,size)   OBJPTR ClassPtr; int size;
 *  OBJPTR GetClass(objectptr)                  OBJPTR objectptr;
 *  OBJPTR ClassSymbol(objectptr)       OBJPTR objectptr;
 */

OBJPTR NewInstance(ClassPtr,size)
        int size;
        OBJPTR ClassPtr;
{
    OBJPTR cptr;
    OBJPTR *iptr;
  if((cptr=(OBJPTR)malloc(size+POINTERSIZE))) {
    iptr=cptr;
    iptr[0]=ClassPtr;
    return(cptr+POINTERSIZE);
  }
  else memfault();
}

FreeInstance(objectptr)
        OBJPTR *objectptr;
{
        char *ptr;

        ptr=objectptr;
        ptr=ptr-POINTERSIZE;
        free(ptr);
}

OBJPTR ClassSymbol(objectptr)
  OBJPTR *objectptr;
{
    char *ptr;
  if(objectptr == NULL) return(NULL);
  ptr=objectptr;
  ptr=ptr-POINTERSIZE;
  objectptr=ptr;
  return(objectptr[0]);
}

OBJPTR GetClass(objectptr)
  OBJPTR *objectptr;
{
    char *ptr;
  if(objectptr == NULL) return(NULL);
  ptr=objectptr;
  ptr=ptr-POINTERSIZE;
  objectptr=ptr;
  objectptr=objectptr[0];
  return(objectptr[OBJPR]);
}
