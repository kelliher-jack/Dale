#define  HASHSIZE  20
/*  HASHSIZE  size of hash table (number of buckets) */

#define  LNKPTR    0
#define  SIZE      0
#define  STRPTR    1
#define  KEY       1
#define  OBJPR     2
#define  VALUE     2

/* LINK FILES:  HASH,STDIO & SYMTBL. */


/* To be comptible with small C  links in a linked list will be
 * represented by pointer arrays of size 3.
 * LNKPTR indexes to the next link pointer,
 * STRPTR indexes to a litteral string pointer &
 * OBJPR  also indexes a pointer.
 */

/*  our representation of the hash table:
 *
 *  +----+
 *  | id | -----------------------\
 *  +----+                        |
 *     |         bucket array     v
 *     |           +-----+  IDSCAN(head,id)
 *     v           |     |   ^    |
 *   HASH(id)      +-----+   |    |
 *     |           |     |   |    v
 *     |           +-----+ head +-----+    +-----+
 *     \---------->|  @--|----->|  @--|--->|     |
 *                 +-----+      +-----+    +-----+
 *                 |     |  /---|--@  |    |     |
 *                 +-----+  |   +-----+    +-----+
 *                 |     |  |   |     |    |     |
 *                 +-----+  |   +-----+    +-----+
 *                 |     |  |      |
 *                 +-----+  |      |
 *                    .     |      v
 *                    :     |   +-----+
 *                 +-----+  |   |     |
 *                 |     |  |   |     |
 *                 +-----+  |   |     |
 *                          |   +-----+
 *  literal table           |
 *  +---+---+---+---+---+   |
 *  |    idname     | 0 |   |
 *  +---+---+---+---+---+   |
 *  |  id   | 0 |           |
 *  +---+---+---+---+       |
 *  |   |   |   | 0 |<------/
 *  +---+---+---+---+
 *
 */
