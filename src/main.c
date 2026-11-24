#include "../include/allocator.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(void) {
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
	if ((test = callocate(&a, 8, 16)) == NULL && errno == ENOMEM) {
		fprintf(stderr, "Allocation failed: %s\n", strerror(errno));
	}

	return 0;
}
