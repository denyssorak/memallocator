#include "../include/allocator.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
	printf("sbrk(0) : %p\n", sbrk(0));

	Allocator a;
	initAllocator(&a, 128);
	printf("%zu, head : %p\n", a.totalsize, a.head);
	/*
	printf("must have one block: ");
	FreeBlock *curr = a.head;
	while (curr != NULL) {
		printf("block\t");
		curr = curr->next;
	}
	printf("\n");
	*/

	void *test;
	if ((test = allocate(&a, 16)) == NULL && errno == ENOMEM) {
		fprintf(stderr, "Allocation failed: %s\n", strerror(errno));
	}

	printf("sbrk(0) : %p\n", sbrk(0));
	printf("%zu, head : %p\n", a.totalsize, a.head);

	return 0;
}
