#include "../include/allocator.h"
#include <stdio.h>

void initAllocator(Allocator *a, size_t newtotalsize) {
	a->totalsize = newtotalsize;
	a->head = (FreeBlock *)malloc(newtotalsize);
	a->head->size = newtotalsize;
	a->head->next = NULL;
}

void *allocate(Allocator *a, size_t size) {
	if (a->totalsize < size + sizeof(BlockLabel)) {
		printf("totalsize < size + BlockLabel size.\n");
		return NULL;
	}
	FreeBlock *curr = a->head;
	FreeBlock *prev = NULL;
	// finding a suitable chunk of memory
	while (curr != NULL) {
		if (curr->size >= size)
			break;
		prev = curr;
		curr = curr->next;
	}
	if (curr == NULL) {
		printf("suitable block not found.\n");
		return NULL;
	}
	FreeBlock *restOfList = curr->next;
	size_t leftover = curr->size - size;
	size_t consumed;

	if (leftover >= sizeof(FreeBlock)) {
		char *splitAddr = (char *)curr + sizeof(BlockLabel) + size;
		FreeBlock *splitBlock = (FreeBlock *)splitAddr;
		splitBlock->size = leftover - sizeof(BlockLabel);
		splitBlock->next = restOfList;

		if (prev == NULL) {
			a->head = splitBlock;
		} else {
			prev->next = splitBlock;
		}

		consumed = size + sizeof(BlockLabel);
	} else {
		if (prev == NULL) {
			a->head = restOfList;
		} else {
			prev->next = restOfList;
		}

		consumed = sizeof(BlockLabel) + curr->size;
	}

	// creating a label for the future allocated memory
	BlockLabel *label = (BlockLabel *)curr;
	initBlockLabel(label, size);

	// creating a pointer to the chunk of memory to give to the user
	void *userPtr = (void *)(label + 1);

	a->totalsize -= consumed;

	return userPtr;
}

void deallocate(Allocator *a, void **mem) {
	if (*mem == NULL)
		return;
	BlockLabel *label = (BlockLabel *)*mem - 1;

	size_t size = label->size;

	FreeBlock *newBlock = (FreeBlock *)label;
	initFreeBlock(newBlock, size);
	newBlock->next = a->head;
	a->head = newBlock;

	a->totalsize += size + sizeof(BlockLabel);
	*mem = NULL;
}
