#include "stdio.h"
#include "dale.h"

extern dump(int table[]);

OBJPTR iadd(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self,*ptr,*anInteger;

	self = parameters[0];
	anInteger = parameters[1];
	ptr = NewInstance(dINTEGER,sizeof(int));
	ptr[0] = (OBJPTR)((int)self[0] + (int)anInteger[0]);
	return(ptr);
}
	
OBJPTR isub(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self,*ptr,*anInteger;

	self = parameters[0];
	anInteger = parameters[1];
	ptr = NewInstance(dINTEGER,sizeof(int));
	ptr[0] = (OBJPTR)((int)self[0] - (int)anInteger[0]);
	return(ptr);
}
	
OBJPTR imult(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self,*ptr,*anInteger;

	self = parameters[0];
	anInteger = parameters[1];
	ptr = NewInstance(dINTEGER,sizeof(int));
	ptr[0] = (OBJPTR)((int)self[0] * (int)anInteger[0]);
	return(ptr);
}
	
OBJPTR idiv(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self,*ptr,*anInteger;

	self = parameters[0];
	anInteger = parameters[1];
	ptr = NewInstance(dINTEGER,sizeof(int));
	ptr[0] = (OBJPTR)((int)self[0] / (int)anInteger[0]);
	return(ptr);
}

OBJPTR imod(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self,*ptr,*anInteger;

	self = parameters[0];
	anInteger = parameters[1];
	ptr = NewInstance(dINTEGER,sizeof(int));
	ptr[0] = (OBJPTR)((int)self[0] % (int)anInteger[0]);
	return(ptr);
}

OBJPTR iequ(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self,*anInteger;
	BOOLEAN *ptr;
	self = parameters[0];
	anInteger = parameters[1];
	ptr = (BOOLEAN *)NewInstance(dBOOLEAN,sizeof(BOOLEAN));
	ptr[0] = (BOOLEAN)((int)self[0] == (int)anInteger[0]);
	return((OBJPTR)ptr);
}

OBJPTR inotequ(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self,*anInteger;
	BOOLEAN *ptr;
	self = parameters[0];
	anInteger = parameters[1];
	ptr = (BOOLEAN *)NewInstance(dBOOLEAN,sizeof(BOOLEAN));
	ptr[0] = (BOOLEAN)((int)self[0] != (int)anInteger[0]);
	return((OBJPTR)ptr);
}

OBJPTR iless(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self,*anInteger;
	BOOLEAN *ptr;
	self = parameters[0];
	anInteger = parameters[1];
	ptr = (BOOLEAN *)NewInstance(dBOOLEAN,sizeof(BOOLEAN));
	ptr[0] = (BOOLEAN)((int)self[0] < (int)anInteger[0]);
	return((OBJPTR)ptr);
}

OBJPTR ilessequ(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self,*anInteger;
	BOOLEAN *ptr;
	self = parameters[0];
	anInteger = parameters[1];
	ptr = (BOOLEAN *)NewInstance(dBOOLEAN,sizeof(BOOLEAN));
	ptr[0] = (BOOLEAN)((int)self[0] <= (int)anInteger[0]);
	return((OBJPTR)ptr);
}

OBJPTR igreater(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self,*anInteger;
	BOOLEAN *ptr;
	self = parameters[0];
	anInteger = parameters[1];
	ptr = (BOOLEAN *)NewInstance(dBOOLEAN,sizeof(BOOLEAN));
	ptr[0] = (BOOLEAN)((int)self[0] > (int)anInteger[0]);
	return((OBJPTR)ptr);
}

OBJPTR inotless(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self,*anInteger;
	BOOLEAN *ptr;
	self = parameters[0];
	anInteger = parameters[1];
	ptr = (BOOLEAN *)NewInstance(dBOOLEAN,sizeof(BOOLEAN));
	ptr[0] = (BOOLEAN)((int)self[0] >= (int)anInteger[0]);
	return((OBJPTR)ptr);
}

OBJPTR iincr(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *ptr;
	OBJPTR *self;

	self = parameters[0];
	ptr = NewInstance(dINTEGER,sizeof(int));
	ptr[0] = (OBJPTR)((int)self[0] + 1);
	return(ptr);
}

OBJPTR idecr(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *ptr;
	OBJPTR *self;

	self = parameters[0];
	ptr = NewInstance(dINTEGER,sizeof(int));
	ptr[0] = (OBJPTR)((int)self[0] - 1);
	return(ptr);
}

OBJPTR iprint(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self;

	self = parameters[0];
	printf("%d",self[0]);
	return(parameters[0]);
}

OBJPTR ilprint(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self;

	self = parameters[0];
	printf("%d\n",self[0]);
	return(parameters[0]);
}

OBJPTR bprint(parameters)
	OBJPTR *parameters[];
{
	BOOLEAN *self;
	self = (BOOLEAN *)parameters[0];
	if(*self) printf("TRUE");
	else printf("FALSE");
	return(parameters[0]);
}

OBJPTR slprint(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *self;

	self = parameters[0];
	printf("%s\n",self);
	return(parameters[0]);
}

OBJPTR oprint(parameters)
	OBJPTR *parameters[];
{
	printf("[%x]",parameters[0]);
	return(parameters[0]);
}

OBJPTR new(parameters)
	OBJPTR *parameters[];
	/* self is a class parameter[0], size is parameter[1] */
{
	OBJPTR instance;

	instance = NewInstance(parameters[0],(int)parameters[1] * POINTERSIZE);
	return(instance);
}

OBJPTR assign(parameters)
	OBJPTR *parameters[];
{
	OBJPTR *address;

	address = parameters[0][0];
	*address = parameters[1];
	return(parameters[1]);
}

OBJPTR sysdump(parameters)
	OBJPTR *parameters[];
{
	BOOLEAN *ptr;
	dump(table);
	ptr = (BOOLEAN *)NewInstance(dBOOLEAN,sizeof(BOOLEAN));
	ptr[0] = 1;
	return((OBJPTR)ptr);
}
