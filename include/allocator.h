#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "freeblock.h"

typedef struct {
	size_t totalsize;
	FreeBlock *head;
} Allocator;

void initAllocator(Allocator *a, size_t newtotalsize);

void *allocate(Allocator *a, size_t size);

void deallocate(Allocator *a, void **mem);

void merge(Allocator *a, FreeBlock *b);

void *reallocate(Allocator *a, void **mem, size_t newsize);

void *callocate(Allocator *a, size_t n, size_t size);

#endif
