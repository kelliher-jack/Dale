/*
 * lists.h — doubly-linked list of OBJPTR items for the Dale interpreter
 *
 * A LIST node is an OBJPTR (char *) that is cast to a LIST pointer.
 * The node is allocated with NewInstance() and lives in the Dale
 * object model, so ClassSymbol() works on it just like any other
 * Dale object.
 */

#ifndef LISTS_H
#define LISTS_H

/* OBJPTR is defined in dale.h; include it before this header */
#include "dale.h"

/* ---- type ---- */
typedef struct listnode {
    OBJPTR item;      /* the Dale object stored in this node */
    OBJPTR class;     /* ClassSymbol(node->class) identifies the Dale class */
    struct listnode *next;  /* next node in the doubly-linked list */
    struct listnode *prev;  /* previous node */
} LISTNODE;

typedef LISTNODE *LIST;

#define NULLIST (LIST)NULL

/* ---- access macros (K&R style, no prototype warnings) ---- */

#define NEXT(l)     ((l) ? ((l)->next) : (LIST)NULL)
#define PREV(l)     ((l) ? ((l)->prev) : (LIST)NULL)
#define ITEM(l)     ((l) ? ((l)->item) : (OBJPTR)NULL)
#define CLASS(l)    ((l) ? ((l)->class) : (OBJPTR)NULL)

/* ---- size helper for NewInstance() ---- */
#define LISTSIZE    (sizeof(LISTNODE))

/* ---- function prototypes ---- */

extern OBJPTR place();
extern LIST ninsert();
extern LIST ExprToList();
extern OBJPTR GetValPtr();

/* allocate a new LIST node and zero it */
#define NewExprList(list) \
    ((list) = (LIST)NewInstance(dEXPRLIST, LISTSIZE), \
     (list)->item = NULL, \
     (list)->class = NULL, \
     (list)->next = NULL, \
     (list)->prev = NULL)

#endif /* LISTS_H */
