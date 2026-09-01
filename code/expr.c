#include "stdio.h"
#include "lists.h"
#include "dale.h"

#define NewExprList(list) ((list) = \
						(LIST)NewInstance(dEXPRLIST,sizeof(LIST)), \
						(list)->next = NULL, \
						(list)->prev = NULL, \
						(list)->item = NULL) \

LIST ExprToList(expr)
	OBJPTR *expr;
{
	int i;
	LIST list,lastnode,node;

	NewExprList(list);
	lastnode = list;
	for(i=0;expr[i] != NULL;i++) {
		NewExprList(node);
		place(expr[i],node);
		ninsert(node,lastnode);
		lastnode = node;
		if(ClassSymbol(expr[i]) == dEXPR)
			place(ExprToList(expr[i]),node);
	}
	return(NEXT(list));
}

/* addrprint(node)
	LIST node;
{
	if(ClassSymbol(node->item) == dBLOCK)
		printf("BLOCK");
	else if(ClassSymbol(node->item) == dEXPRLIST)
		printf("EXPR");
	else
		printf("%x",node->item);
} */
