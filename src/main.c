#include "../include/allocator.h"
#include <stdio.h>

int main(void) {
	Allocator a;
	initAllocator(&a, 128);
	printf("%zu, head : %p\n", a.totalsize, a.head);
	printf("must have one block: ");
	FreeBlock *curr = a.head;
	while (curr != NULL) {
		printf("block\t");
		curr = curr->next;
	}
	printf("\n");

	void *test1 = NULL;

	void *test2 = allocate(&a, 16);
	void *temp = reallocate(&a, &test1, 16);
	if (temp == NULL) {
		printf("reallocate failed\n");
		return 0;
	} else {
		test1 = temp;
	}
	BlockLabel *label1 = (BlockLabel *)((char *)test1 - sizeof(BlockLabel));
	printf("16 -> %zu\n", label1->size);
	printf("%zu, head : %p\n", a.totalsize, a.head);
	return 1;
}
