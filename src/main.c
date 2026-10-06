#include "../include/allocator.h"
#include <stdio.h>

int main(void) {
	Allocator a;
	initAllocator(&a, 128);
	/*
		printf("Initial List Head: %p\n", a.head);

		printf("Allocating 16 Bytes\n");
		void *test1 = allocate(&a, 16);
		printf("List Head: %p\t User Memory: %p\n", a.head, test1);

		printf("Deallocating those 16 Bytes\n");
		deallocate(&a, &test1);
		printf("List Head: %p\t User Memory: %p\n", a.head, test1);

		printf("Allocating 20 Bytes (Should use a different memory address)\n");
		test1 = allocate(&a, 20);
		printf("List Head: %p\t User Memory: %p\n", a.head, test1);
	*/

	FreeBlock *curr = a.head;
	while (curr != NULL) {
		printf("block before\n");
		curr = curr->next;
	}
	printf("%zu\n", a.totalsize);
	void *test1 = allocate(&a, 16);
	printf("%zu\n", a.totalsize);
	deallocate(&a, &test1);
	printf("%zu\n", a.totalsize);
	curr = a.head;
	while (curr != NULL) {
		printf("block after \n");
		curr = curr->next;
	}

	return 0;
}
