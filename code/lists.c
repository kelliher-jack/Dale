/*
 * lists.c — implementation of the doubly-linked list functions
 *           for the Dale interpreter.
 *
 * Function bodies moved here from lists.h so that each function
 * is defined exactly once across the compilation unit.
 */

#include "lists.h"

/* place(item, node) — store 'item' in 'node' */
OBJPTR place(item, node)
    OBJPTR item;
    LIST   node;
{
    if (node) node->item = item;
    return item;
}

/* ninsert(node, hear) — append 'node' after 'hear' in the list */
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
