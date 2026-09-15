#ifndef FREEBLOCK_H
#define FREEBLOCK_H

#include <stdlib.h>

typedef struct FreeBlock {
	size_t size;
	struct FreeBlock *next;
} FreeBlock;

void initFreeBlock(FreeBlock *b, size_t newsize);

// adding struct for the memory label
typedef struct {
	size_t size;
} BlockLabel;

inline void initBlockLabel(BlockLabel *b, size_t newsize) {
	b->size = newsize;
};

#endif
