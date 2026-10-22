#include "../include/allocator.h"
#include <stdio.h>

int main(void) {
	Allocator a;
	initAllocator(&a, 128);

	FreeBlock *curr = a.head;
	while (curr != NULL) {
		printf("block before\n");
		curr = curr->next;
	}
	printf("%zu\n", a.totalsize);

	void *test1 = allocate(&a, 16);
	void *test2 = allocate(&a, 16);
	deallocate(&a, &test1);
	deallocate(&a, &test2);

	printf("%zu\n", a.totalsize);

	curr = a.head;
	while (curr != NULL) {
		printf("block after\n");
		curr = curr->next;
	}

	return 0;
}
