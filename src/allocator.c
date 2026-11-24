#include "../include/allocator.h"
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void initAllocator(Allocator *a, size_t newtotalsize) {
	a->totalsize = newtotalsize;
	a->head = (FreeBlock *)malloc(newtotalsize);
	a->head->size = newtotalsize;
	a->head->next = NULL;
}

void *allocate(Allocator *a, size_t size) {
	if (a->totalsize < size + sizeof(BlockLabel)) {
		// there is not enough space to allocate a block
		errno = ENOMEM;
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
		// no suitable block was found
		errno = ENOMEM;
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
		if (curr == (FreeBlock *)((char *)b + sizeof(BlockLabel) + b->size)) {
			// if b hasn't been merged before
			// we simply make b bigger
			// and chain it into the freelist
			if (merged == 0) {
				// check if the block is the head or not
				if (prev != NULL) {
					prevOfB = prev;
					prev->next = b;
				} else {
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
			b->size += curr->size + sizeof(BlockLabel);
			prev = curr;
			curr = curr->next;
		}
		// now if curr is the block behind b
		else if (b == (FreeBlock *)((char *)curr + sizeof(BlockLabel) + curr->size)) {
			FreeBlock *temp = curr;
			curr = curr->next;
			// if b hasn't been merged
			// we just indicated that now it is
			if (merged == 0) {
				merged = 1;
				prev = temp;
			}
			// if b has been merged previously
			// we have made a temp ptr to keep curr running forward
			// and we unlink temp from the list
			// and link it in front of b, increasing temp's size
			// and making b = temp
			else {
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
			temp->size += b->size + sizeof(BlockLabel);
			b = temp;
		} else {
			prev = curr;
			curr = curr->next;
		}
	}
	if (merged == 0) {
		b->next = a->head;
		a->head = b;
	}

	return;
}

void *reallocate(Allocator *a, void **mem, size_t newsize) {
	// initial checks
	if (*mem == NULL) {
		if (newsize <= 0) {
			return NULL;
		}
		return allocate(a, newsize);
	}
	if (newsize <= 0) {
		deallocate(a, mem);
		return NULL;
	}

	// find the label of the block we are working with
	BlockLabel *label = (BlockLabel *)*mem - 1;
	// reallocating less memory
	if (newsize + sizeof(FreeBlock) <= label->size) {
		// calculate the leftover size
		size_t leftover = label->size - newsize - sizeof(BlockLabel);
		// create a label for the left over so we can simply pass it into deallocate()
		BlockLabel *leftoverlabel = (BlockLabel *)((char *)*mem + newsize);
		initBlockLabel(leftoverlabel, leftover);

		// now the ptr that is passed into deallocate()
		void *leftoverptr = (void *)(leftoverlabel + 1);
		deallocate(a, &leftoverptr);

		// change the size of the main label to the new size
		label->size = newsize;
		// return the ptr
		return *mem;
	} else if (newsize > label->size) {
		// find what is located right after this block
		FreeBlock *curr = a->head;
		FreeBlock *prev = NULL;

		while (curr != NULL) {
			// if curr is the block in front of main block
			if (curr == (FreeBlock *)((char *)label + sizeof(BlockLabel) + label->size) && curr->size >= (newsize - label->size + sizeof(FreeBlock))) {
				FreeBlock *splitBlock = (FreeBlock *)((char *)curr + (newsize - label->size));
				splitBlock->size = curr->size - (newsize - label->size);
				splitBlock->next = curr->next;
				if (prev != NULL) {
					prev->next = splitBlock;
				} else {
					a->head = splitBlock;
				}

				a->totalsize -= (newsize - label->size);
				label->size = newsize;

				return *mem;
			}
			prev = curr;
			curr = curr->next;
		}
		void *newMem = allocate(a, newsize);
		if (newMem == NULL) {
			return NULL;
		}
		memcpy(newMem, *mem, label->size);
		deallocate(a, mem);
		return newMem;
	}
	return *mem;
}

void *callocate(Allocator *a, size_t n, size_t size) {
	// Initial checks
	if (n == 0 || size == 0) {
		return NULL;
	}
	if (SIZE_MAX / size < n) {
		// not enough space for the allocation
		errno = ENOMEM;
		return NULL;
	}

	size_t total = n * size;

	void *mem = allocate(a, total);
	if (mem == NULL) {
		return NULL;
	}

	char *curr = (char *)mem;
	for (size_t i = 0; i < total; i++) {
		*curr = 0;
		curr += 1;
	}

	return mem;
}
