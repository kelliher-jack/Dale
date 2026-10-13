/*
 * dump.c — human-readable dump of the Dale symbol table for debugging.
 *
 * Walks the hash table and prints every class with its superclass,
 * instance variables, methods, and code blocks (expressions).
 *
 * (C) 1984 Jack S. Kelliher  — 64-bit port October 2026
 */

#include "stdio.h"
#include "dale.h"
#include "hash.h"

/* ---- forward declarations (correct prototypes for 64-bit) ---- */

static void printclass(OBJPTR *ptr);
static void printvar(OBJPTR *tmp, int size);
static void printblock(OBJPTR *ptr, int space);
static void printexpr(OBJPTR *ptr, int space);
static void printspec(char *msg, OBJPTR *ptr, int space);
static void tab(int space);

/* ---- public entry point ---- */

void dump(OBJPTR table[])
{
    int  i, tablesize;
    OBJPTR *current, *ptr;
    OBJPTR class;

    tablesize = (int)(long)table[SIZE];   /* stored as OBJPTR, small int */

    printf("\n");
    printf("===== SYMBOL TABLE DUMP  (size %d) =====\n", tablesize);
    fflush(stdout);

    for (i = 1; i < tablesize; i++) {
        if (table[i] == NULL) continue;

        printf("--- bucket[%d] ---\n", i);

        for (current = (OBJPTR *)table[i]; current; current = (OBJPTR *)current[LNKPTR]) {

            /* symbol name */
            fputs("  ", stdout);
            fputs(current[STRPTR], stdout);

            /* what kind of object is it? */
            class = NULL;
            ptr = (OBJPTR *)current[OBJPR];
            if (ptr) {
                class = ClassSymbol((OBJPTR)ptr);
                fputs("  (", stdout);
                fputs(((OBJPTR *)class)[STRPTR], stdout);
                fputs(")", stdout);
            }
            putchar(EOL);

            /* if it's a class, print its internals */
            if (class == dCLASS && ptr)
                printclass(ptr);
        }
    }
    printf("===== END DUMP =====\n\n");
}

/* ---- print a class definition ---- */

static void printclass(OBJPTR *ptr)
{
    int j, k, l;
    OBJPTR *s, *tmp;

    /* superclass */
    s = (OBJPTR *)ptr[0];
    if (s) {
        fputs("    superclass: ", stdout);
        fputs(s[STRPTR], stdout);
        putchar(EOL);
    }

    /* instance variables */
    tmp = (OBJPTR *)ptr[1];
    if (tmp)
        printvar(tmp, 4);

    /* method tables (domains: INTERNAL, EXTERNAL, XCLASS) */
    for (j = 2; j < 5; j++) {
        if (ptr[j] == NULL) continue;

        s = (OBJPTR *)ptr[j];       /* s -> METHODS object */
        fputs("    METHODS [domain ", stdout);
        putchar(j + '0');
        fputs("]  count=", stdout);
        printf("%d\n", (int)(long)s[0]);

        for (k = 1; k < (int)(long)s[0] * 2 + 1; k += 2) {
            tmp = (OBJPTR *)s[k];   /* tmp -> message template */

            /* print the message pattern */
            tab(6);
            fputs("msg: ", stdout);
            for (l = 0; tmp[l] != NULL; l++) {
                if (tmp[l] < (OBJPTR)4) {
                    /* small integer = parameter slot number */
                    printf("#%d ", (int)(long)tmp[l]);
                } else {
                    fputs((char *)tmp[l], stdout);
                    putchar(' ');
                }
            }
            putchar(EOL);

            /* print the code block or primitive */
            tmp = (OBJPTR *)s[k + 1];  /* tmp -> block or primitive */
            if (ClassSymbol((OBJPTR)tmp) == dBLOCK)
                printblock(tmp, 6);
            else
                printf("      PRIMITIVE call[%d]\n", (int)(long)tmp[0]);
        }
    }
}

/* ---- print instance variable declarations ---- */

static void printvar(OBJPTR *tmp, int size)
{
    int j;
    char num[16];

    tab(size);
    if (tmp == NULL) return;

    fputs("instance vars (count=", stdout);
    _itoa((int)(long)tmp[1], num);
    fputs(num, stdout);
    fputs("):\n", stdout);

    for (j = 2; j < 3 * (int)(long)tmp[1] && tmp != NULL; ) {
        tab(size + 2);
        fputs("  ", stdout);
        fputs((char *)tmp[j++], stdout);   /* name */
        putchar(' ');
        fputs((char *)tmp[j++], stdout);   /* type */
        fputs(" @offset ", stdout);
        _itoa((int)(long)tmp[j++], num);   /* offset */
        fputs(num, stdout);
        putchar(EOL);
    }
}

/* ---- print a code block ---- */

static void printblock(OBJPTR *ptr, int space)
{
    int i;

    tab(space);
    fputs("BLOCK  (exprs=", stdout);
    printf("%d)\n", (int)(long)ptr[0]);

    printvar((OBJPTR *)ptr[1], space + 2);

    for (i = 2; i < (int)(long)ptr[0] + 2; i++)
        printexpr((OBJPTR *)ptr[i], space + 2);
}

/* ---- print an expression list ---- */

static void printexpr(OBJPTR *ptr, int space)
{
    int i;
    OBJPTR *tmp, *cptr;

    if (ptr == NULL) return;

    tab(space);
    fputs("EXPRESSION:\n", stdout);

    i = 0;
    while (ptr[i] != NULL) {
        cptr = (OBJPTR *)ClassSymbol(ptr[i]);

        if (cptr == (OBJPTR *)dBLOCK)
            printblock((OBJPTR *)ptr[i], space + 2);
        else if (cptr == (OBJPTR *)dEXPR)
            printexpr((OBJPTR *)ptr[i], space + 2);
        else if (cptr == (OBJPTR *)dSTRING) {
            tab(space + 2);
            printf("STRING \"%s\"\n", (char *)ptr[i]);
        }
        else if (cptr == (OBJPTR *)dINTEGER) {
            tab(space + 2);
            printf("NUMBER %d\n", (int)(long)((OBJPTR *)ptr[i])[0]);
        }
        else if (cptr == (OBJPTR *)dLABEL)
            printspec("LABEL", (OBJPTR *)ptr[i], space + 2);
        else if (cptr == (OBJPTR *)dGOTO)
            printspec("GOTO", (OBJPTR *)ptr[i], space + 2);
        else if (cptr == (OBJPTR *)dPARAMETER)
            printspec("PARAM", (OBJPTR *)ptr[i], space + 2);
        else if (cptr == (OBJPTR *)dVAR)
            printspec("VAR", (OBJPTR *)ptr[i], space + 2);
        else {
            tmp = (OBJPTR *)ptr[i];
            tab(space + 2);
            fputs((char *)tmp[STRPTR], stdout);
            putchar(EOL);
        }
        i++;
    }
}

/* ---- print a labelled spec (label, goto, parameter, variable) ---- */

static void printspec(char *msg, OBJPTR *ptr, int space)
{
    char num[16];

    tab(space);
    if (msg) {
        fputs(msg, stdout);
        fputs(": ", stdout);
    }
    fputs((char *)ptr[0], stdout);
    fputs(" = ", stdout);
    _itoa((int)(long)ptr[1], num);
    fputs(num, stdout);
    putchar(EOL);
}

/* ---- indent helper ---- */

static void tab(int space)
{
    int i;
    for (i = 0; i < space; i++)
        putchar(' ');
}
