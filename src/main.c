#include "../include/allocator.h"
#include <stdio.h>

int main(void) {
	Allocator a;
	initAllocator(&a, 128);

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

	return 0;
}
