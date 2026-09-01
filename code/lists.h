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

/* ---- function prototypes (K&R style for compatibility) ---- */

/* place(item, node) — store 'item' in 'node' */
OBJPTR place(item, node)
    OBJPTR item;
    LIST   node;
{
    if (node) node->item = item;
    return item;
}

/* insert(node, hear) — append 'node' after 'hear' in the list */
LIST ninsert(node, hear)
    LIST node;
    LIST hear;
{
    if (!node) return NULL;

    node->next = NULL;
    node->prev = hear;

    if (hear) {
        hear->next = node;
    }

    return node;
}

/* NewExprList(list) — allocate a new LIST node and zero it */
#define NewExprList(list) \
    ((list) = (LIST)NewInstance(dEXPRLIST, LISTSIZE), \
     (list)->item = NULL, \
     (list)->class = NULL, \
     (list)->next = NULL, \
     (list)->prev = NULL)

/* ExprToList(expr) — convert a NULL-terminated OBJPTR array into a LIST */
LIST ExprToList(expr)
    OBJPTR *expr;
{
    int i;
    LIST list, lastnode, node;

    NewExprList(list);
    lastnode = list;

    for (i = 0; expr[i] != NULL; i++) {
        NewExprList(node);
        place(expr[i], node);
        ninsert(node, lastnode);
        lastnode = node;

        if (ClassSymbol(expr[i]) == dEXPR) {
            place(ExprToList(expr[i]), node);
        }
    }

    return NEXT(list);
}

/* GetValPtr(node) — extract the OBJPTR stored in a LIST node */
OBJPTR GetValPtr(node)
    LIST   node;
{
    return node ? node->item : NULL;
}

#endif /* LISTS_H */
