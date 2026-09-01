#include "stdio.h"
#include "dale.h"
#include "hash.h"

/*
 * Symbol Table Module
 *
 * FILES: symtbl.c dump.c hash.c
 *
 * EXTERNAL PROCEDURES:
 *  OBJPTR lookup(table,symbol)		OBJPTR table[]; char symbol[];
 *  OBJPTR add(table,symbol)		OBJPTR table[]; char symbol[];
 *	void dump(table)				OBJPTR table[];
 */

/*
 * Lookup - Returns the object pointer for symbol if found,
 * returns NULL otherwise
 */

OBJPTR lookup(table,symbol)
	OBJPTR  table[];
	char symbol[];

{
    int pointer;
    OBJPTR scan();

    return(scan(&table[hash(symbol,(int)table[SIZE])],symbol,&pointer));
}

/*
 * Add - If Table contains an entry for symbol, an error is
 * reported by returning NULL,  else a new entry for symbol is added
 * to table & returns pointer to the bucket
 */

OBJPTR add(table,symbol)
	OBJPTR  table[];
	char symbol[];

{
    OBJPTR bucket,scan();
    OBJPTR *pointer;

    if((bucket=scan(&table[hash(symbol,(int)table[SIZE])],symbol,&pointer)))
      return(bucket);
    else {
      insert(mkname(symbol),NULL,pointer);
      return(*pointer);
    }
}

