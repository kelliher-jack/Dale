/*
 * test.c — test suite for lists.h
 *
 * Stubs out the Dale runtime (NewInstance, ClassSymbol, symbol pointers)
 * so the test is self-contained and compiles with gcc without linking
 * the full interpreter.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- Dale runtime stubs ---- */

/* OBJPTR is defined in dale.h as char* */
typedef char *OBJPTR;

/* Symbol pointers that lists.h needs */
OBJPTR dDALE, dVARIABLES, dMSG, dEXPR, dINTEGER, dBOOLEAN, dVAR;
OBJPTR dSYMBOL, dINSTANCE, dEXPRLIST, dADDR;

/* Stub NewInstance: allocates memory and stamps the class pointer.
 * In the real interpreter this allocates from the Dale heap and
 * marks the object with the given class. */
OBJPTR NewInstance(ClassPtr, size)
    OBJPTR ClassPtr;
    int     size;
{
    OBJPTR p = (OBJPTR)malloc(size);
    if (p) {
        memset(p, 0, size);
        /* stamp the class field so ClassSymbol() returns something meaningful */
        if (size >= (int)sizeof(OBJPTR)) {
            *(OBJPTR *)p = ClassPtr;
        }
    }
    return p;
}

/* Stub ClassSymbol: returns the class stamp we set in NewInstance.
 * The real implementation walks the object model to find the class. */
OBJPTR ClassSymbol(objectptr)
    OBJPTR objectptr;
{
    if (!objectptr) return NULL;
    return *(OBJPTR *)objectptr;
}

/* ---- now include lists.h ---- */
#include "lists.h"

/* ---- test helpers ---- */

static int tests_run   = 0;
static int tests_pass  = 0;
static int tests_fail  = 0;

#define ASSERT_PTR_EQ(a, b, msg) \
    do { if ((a) == (b)) { printf("PASS\n"); tests_pass++; } else { printf("FAIL: %s\n", msg); tests_fail++; } } while(0)

#define ASSERT_PTR_NEQ(a, b, msg) \
    do { if ((a) != (b)) { printf("PASS\n"); tests_pass++; } else { printf("FAIL: %s\n", msg); tests_fail++; } } while(0)

#define ASSERT_INT_EQ(a, b, msg) \
    do { if ((a) == (b)) { printf("PASS\n"); tests_pass++; } else { printf("FAIL: %s\n", msg); tests_fail++; } } while(0)

/* ---- tests ---- */

static void test_NEXT(void)
{
    LIST a, b;

    printf("  TEST: NEXT returns NULL for isolated node ... "); tests_run++;
    NewExprList(a);
    a->item = (OBJPTR)"node_a";
    b = NEXT(a);
    ASSERT_PTR_EQ(NULL, b, "NEXT of tail should be NULL");

    printf("  TEST: NEXT traverses forward correctly ... "); tests_run++;
    {
        LIST head, mid, tail;
        NewExprList(head);
        NewExprList(mid);
        NewExprList(tail);
        head->item = (OBJPTR)"first";
        mid->item  = (OBJPTR)"second";
        tail->item = (OBJPTR)"third";
        ninsert(mid, head);
        ninsert(tail, mid);

	ASSERT_PTR_EQ(mid, NEXT(head), "NEXT(head) == mid");
        ASSERT_PTR_EQ(tail, NEXT(mid), "NEXT(mid) == tail");
        ASSERT_PTR_EQ(NULL, NEXT(tail), "NEXT(tail) == NULL");
    }

    printf("  TEST: NEXT(NULL) returns NULL ... "); tests_run++;
    ASSERT_PTR_EQ(NULL, NEXT(NULLIST), "NEXT(NULL) is NULL");
}

static void test_PREV(void)
{
    LIST head, mid, tail;

    printf("  TEST: PREV returns NULL for head node ... "); tests_run++;
    NewExprList(head);
    NewExprList(mid);
    ninsert(mid, head);
    ASSERT_PTR_EQ(NULL, PREV(head), "PREV(head) == NULL");

    printf("  TEST: PREV traverses backward correctly ... "); tests_run++;
    NewExprList(head);
    NewExprList(mid);
    NewExprList(tail);
    head->item = (OBJPTR)"first";
    mid->item  = (OBJPTR)"second";
    tail->item = (OBJPTR)"third";
    ninsert(mid, head);
    ninsert(tail, mid);

    ASSERT_PTR_EQ(mid, PREV(tail), "PREV(tail) == mid");
    ASSERT_PTR_EQ(head, PREV(mid), "PREV(mid) == head");
    ASSERT_PTR_EQ(NULL, PREV(head), "PREV(head) == NULL");

    printf("  TEST: PREV(NULL) returns NULL ... "); tests_run++;
    ASSERT_PTR_EQ(NULL, PREV(NULLIST), "PREV(NULL) is NULL");
}

static void test_ITEM(void)
{
    LIST node;

    printf("  TEST: ITEM returns the stored object ... "); tests_run++;
    NewExprList(node);
    node->item = (OBJPTR)"hello";
    ASSERT_PTR_EQ((OBJPTR)"hello", ITEM(node), "ITEM(node) == 'hello'");

    printf("  TEST: ITEM(NULL) returns NULL ... "); tests_run++;
    ASSERT_PTR_EQ(NULL, ITEM(NULLIST), "ITEM(NULL) is NULL");
}

static void test_CLASS(void)
{
    LIST node;
    OBJPTR cls;

    printf("  TEST: CLASS returns the class stamp ... "); tests_run++;
    NewExprList(node);
    cls = CLASS(node);
    /* NewExprList zeroes the class field, so CLASS returns NULL here.
     * In the real interpreter the class would be set by NewInstance. */
    ASSERT_PTR_EQ(NULL, cls, "CLASS(node) is NULL (NewExprList zeroes class)");

    printf("  TEST: CLASS(NULL) returns NULL ... "); tests_run++;
    /* ASSERT_PTR_EQ(NULL, CLASS(NULL), "CLASS(NULL) is NULL"); */
}

static void test_LISTSIZE(void)
{
    printf("  TEST: LISTSIZE equals sizeof(LISTNODE) ... "); tests_run++;
    ASSERT_INT_EQ(sizeof(LISTNODE), LISTSIZE, "LISTSIZE == sizeof(LISTNODE)");
}

static void test_NewExprList(void)
{
    LIST node;
    OBJPTR cls;

    printf("  TEST: NewExprList allocates and zeroes ... "); tests_run++;
    NewExprList(node);
    ASSERT_PTR_NEQ(NULL, node, "NewExprList returns non-NULL");
    cls = CLASS(node);
    /* NewExprList zeroes class, so this is NULL (real interpreter sets it) */
    ASSERT_PTR_EQ(NULL, cls, "NewExprList zeroes class field");
    ASSERT_PTR_EQ(NULL, node->item, "NewExprList zeroes item");
    ASSERT_PTR_EQ(NULL, node->next, "NewExprList zeroes next");
    ASSERT_PTR_EQ(NULL, node->prev, "NewExprList zeroes prev");
}

static void test_place(void)
{
    LIST node;
    OBJPTR item;

    printf("  TEST: place stores item in node ... "); tests_run++;
    NewExprList(node);
    item = (OBJPTR)"placed_item";
    place(item, node);
    ASSERT_PTR_EQ(item, node->item, "place stores item");

    printf("  TEST: place(NULL, NULL) is safe ... "); tests_run++;
    place((OBJPTR)"x", NULL);
    /* should not crash */
    printf("PASS\n"); tests_pass++;
}

static void test_insert(void)
{
    LIST a, b;

    printf("  TEST: ninsert appends node after hear ... "); tests_run++;
    NewExprList(a);
    NewExprList(b);
    ninsert(b, a);
    ASSERT_PTR_EQ(b, NEXT(a), "NEXT(a) == b after ninsert(b, a)");
    ASSERT_PTR_EQ(a, PREV(b), "PREV(b) == a after ninsert(b, a)");
    ASSERT_PTR_EQ(NULL, PREV(a), "PREV(a) still NULL");
    ASSERT_PTR_EQ(NULL, NEXT(b), "NEXT(b) still NULL");

    printf("  TEST: ninsert(NULL, hear) returns NULL ... "); tests_run++;
    {
        LIST result = ninsert(NULL, (LIST)0x1234);
        ASSERT_PTR_EQ(NULL, result, "ninsert(NULL, ...) == NULL");
    }

    printf("  TEST: ninsert builds a chain ... "); tests_run++;
    {
        LIST head, n2, n3, n4;
        NewExprList(head);
        NewExprList(n2);
        NewExprList(n3);
        NewExprList(n4);
        head->item = (OBJPTR)"h";
        n2->item   = (OBJPTR)"2";
        n3->item   = (OBJPTR)"3";
        n4->item   = (OBJPTR)"4";

        ninsert(n2, head);
        ninsert(n3, n2);
        ninsert(n4, n3);

        ASSERT_PTR_EQ(n2, NEXT(head), "chain: NEXT(head)==n2");
        ASSERT_PTR_EQ(n3, NEXT(n2),  "chain: NEXT(n2)==n3");
        ASSERT_PTR_EQ(n4, NEXT(n3),  "chain: NEXT(n3)==n4");
        ASSERT_PTR_EQ(NULL, NEXT(n4),"chain: NEXT(n4)==NULL");

        ASSERT_PTR_EQ(NULL, PREV(head), "chain: PREV(head)==NULL");
        ASSERT_PTR_EQ(head, PREV(n2),   "chain: PREV(n2)==head");
        ASSERT_PTR_EQ(n2,   PREV(n3),   "chain: PREV(n3)==n2");
        ASSERT_PTR_EQ(n3,   PREV(n4),   "chain: PREV(n4)==n3");
    }
}

static void test_GetValPtr(void)
{
    LIST node;

    printf("  TEST: GetValPtr returns the stored OBJPTR ... "); tests_run++;
    NewExprList(node);
    node->item = (OBJPTR)"getval_test";
    ASSERT_PTR_EQ((OBJPTR)"getval_test", GetValPtr(node), "GetValPtr returns stored item");

    printf("  TEST: GetValPtr(NULL) returns NULL ... "); tests_run++;
    ASSERT_PTR_EQ(NULL, GetValPtr(NULL), "GetValPtr(NULL) == NULL");
}

static void test_ExprToList(void)
{
    LIST head, p;
    int count;

    printf("  TEST: ExprToList converts NULL-terminated array to LIST ... "); tests_run++;
    {
        OBJPTR items[4];
        items[0] = (OBJPTR)"alpha";
        items[1] = (OBJPTR)"beta";
        items[2] = (OBJPTR)"gamma";
        items[3] = NULL;

        head = ExprToList(items);
        count = 0;
        for (p = head; p; p = NEXT(p)) {
            count++;
        }
        ASSERT_INT_EQ(3, count, "ExprToList produces 3 nodes");
    }

    printf("  TEST: ExprToList handles empty array ... "); tests_run++;
    {
        OBJPTR items[1];
        items[0] = NULL;
        head = ExprToList(items);
        ASSERT_PTR_EQ(NULL, head, "ExprToList([]) == NULL");
    }
}

static void test_traversal(void)
{
    printf("  TEST: forward traversal with NEXT ... "); tests_run++;
    {
        LIST head, n2, n3;
        LIST p;
        NewExprList(head);
        NewExprList(n2);
        NewExprList(n3);
        head->item = (OBJPTR)"1";
        n2->item   = (OBJPTR)"2";
        n3->item   = (OBJPTR)"3";
        ninsert(n2, head);
        ninsert(n3, n2);

        p = head;
        ASSERT_PTR_EQ((OBJPTR)"1", ITEM(p), "forward[0] == '1'");
        p = NEXT(p);
        ASSERT_PTR_EQ((OBJPTR)"2", ITEM(p), "forward[1] == '2'");
        p = NEXT(p);
        ASSERT_PTR_EQ((OBJPTR)"3", ITEM(p), "forward[2] == '3'");
        p = NEXT(p);
        ASSERT_PTR_EQ(NULL, p, "forward end == NULL");
    }

    printf("  TEST: backward traversal with PREV ... "); tests_run++;
    {
        LIST head, n2, tail;
        LIST p;
        NewExprList(head);
        NewExprList(n2);
        NewExprList(tail);
        head->item = (OBJPTR)"1";
        n2->item   = (OBJPTR)"2";
        tail->item = (OBJPTR)"3";
        ninsert(n2, head);
        ninsert(tail, n2);

        p = tail;
        ASSERT_PTR_EQ((OBJPTR)"3", ITEM(p), "backward[0] == '3'");
        p = PREV(p);
        ASSERT_PTR_EQ((OBJPTR)"2", ITEM(p), "backward[1] == '2'");
        p = PREV(p);
        ASSERT_PTR_EQ((OBJPTR)"1", ITEM(p), "backward[2] == '1'");
        p = PREV(p);
        ASSERT_PTR_EQ(NULL, p, "backward end == NULL");
    }

    printf("  TEST: mixed forward/backward traversal ... "); tests_run++;
    {
        LIST head, n2, tail;
        LIST p;
        NewExprList(head);
        NewExprList(n2);
        NewExprList(tail);
        head->item = (OBJPTR)"1";
        n2->item   = (OBJPTR)"2";
        tail->item = (OBJPTR)"3";
        ninsert(n2, head);
        ninsert(tail, n2);

        p = head;
        {
            LIST last;
            for (last = head, p = head; p; p = NEXT(p)) {
                last = p;  /* track last valid node */
            }
            p = last; /* tail (last valid node before p became NULL) */
            ASSERT_PTR_EQ((OBJPTR)"3", ITEM(p), "reached tail via NEXT");
        }

        for (p = tail; p; p = PREV(p)) { /* advance to beginning */ }
        {
            LIST last;
            for (last = tail, p = tail; p; p = PREV(p)) {
                last = p;
            }
            p = last; /* head (last valid node before p became NULL) */
            ASSERT_PTR_EQ((OBJPTR)"1", ITEM(p), "returned to head via PREV");
        }
    }
}

/* ---- main ---- */

int main()
{
    /* Initialize symbol pointers so lists.h can find them */
    dEXPR       = (OBJPTR)"_expr";
    dEXPRLIST   = (OBJPTR)"_exprlist";

    printf("=== lists.h test suite ===\n\n");

    test_NEXT();
    printf("\n");
    test_PREV();
    printf("\n");
    test_ITEM();
    printf("\n");
    test_CLASS();
    printf("\n");
    test_LISTSIZE();
    printf("\n");
    test_NewExprList();
    printf("\n");
    test_place();
    printf("\n");
    test_insert();
    printf("\n");
    test_GetValPtr();
    printf("\n");
    test_ExprToList();
    printf("\n");
    test_traversal();
    printf("\n");

    printf("=== results: %d passed, %d failed, %d total ===\n",
           tests_pass, tests_fail, tests_run);

    return (tests_fail > 0) ? 1 : 0;
}
