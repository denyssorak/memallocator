#include "../include/allocator.h"

void initAllocator(Allocator *a, size_t newtotalsize) {
	a->totalsize = newtotalsize;
	a->head = (FreeBlock *)malloc(newtotalsize);
	a->head->size = newtotalsize;
	a->head->next = NULL;
}

void *allocate(Allocator *a, size_t size) {
	FreeBlock *curr = a->head;
	FreeBlock *prev;
	// finding a suitable chunk of memory
	while (curr != NULL) {
		if (curr->size >= size + sizeof(BlockLabel))
			break;
		prev = curr;
		curr = curr->next;
	}
	if (curr == NULL)
		return NULL;
	prev->next = curr->next;

	// creating a label for the future allocated memory
	BlockLabel *label = (BlockLabel *)curr;
	initBlockLabel(label, size);

	// creating a pointer to the chunk of memory to give to the user
	void *userPtr = (void *)(label + 1);

	return userPtr;
}

void deallocate(Allocator *a, void **mem) {
	BlockLabel *label = (BlockLabel *)*mem - 1;

	int size = label->size;

	FreeBlock *newBlock = (FreeBlock *)label;
	initFreeBlock(newBlock, size);
	newBlock->next = a->head;
	a->head = newBlock;

	mem = NULL;
}
