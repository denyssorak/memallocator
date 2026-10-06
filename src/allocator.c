#include "../include/allocator.h"
#include <stdbool.h>
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
	printf("%zu\n", size);

	FreeBlock *newBlock = (FreeBlock *)label;
	initFreeBlock(newBlock, size);
	//  newBlock->next = a->head;
	//  a->head = newBlock;
	merge(a, newBlock);

	a->totalsize += size + sizeof(BlockLabel);
	*mem = NULL;
}

void merge(Allocator *a, FreeBlock *b) {
	FreeBlock *curr = a->head;
	FreeBlock *prev = NULL;
	bool merged = 0;
	FreeBlock *prevOfB = NULL; // is used to merge

	while (curr != NULL) {
		// if curr is the block in front of b
		if (curr == (FreeBlock *)((char *)b + sizeof(FreeBlock) + b->size)) {
			// if b hasn't been merged before
			// we simply make b bigger
			// and chain it into the freelist
			if (merged == 0) {
				// check if the block is the head or not
				if (prev != NULL) {
					prevOfB = prev;
					// if it is not
					prev->next = b;
				} else {
					// if it is then b is the new head
					a->head = b;
				}
				b->next = curr->next;
				merged = 1;
			}
			// if b has been merged already
			// we unchain the found block to connect to b
			// to avoid chaining b again
			else {
				if (prev != NULL) {
					prev->next = curr->next;
				} else {
					a->head = curr->next;
				}
			}
			b->size += curr->size;
			prev = curr;
			curr = curr->next;
		}
		// now if curr is the block behind b
		else if (b == (FreeBlock *)((char *)curr + sizeof(FreeBlock) + curr->size)) {
			FreeBlock *temp = curr;
			curr = curr->next;
			// if b hasn't been merged
			// we just indicated that now it is
			if (merged == 0) {
				merged = 1;
			} else {
				if (prev != NULL) {
					prev->next = temp->next;
				} else {
					a->head = temp->next;
				}
				if (prevOfB != NULL) {
					prevOfB->next = temp;
				} else {
					a->head = temp;
				}
				temp->next = b->next;
			}
			temp->size += b->size;
			b = temp;
		} else {
			prev = curr;
			curr = curr->next;
		}
	}
}
