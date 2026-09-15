#include "../include/freeblock.h"

void initFreeBlock(FreeBlock *b, size_t newsize) {
	b = (FreeBlock *)malloc(newsize);
	b->size = newsize;
	b->next = NULL;
}
