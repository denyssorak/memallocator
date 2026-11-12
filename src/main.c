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
	/*
		TESTING merge()
		void *test1 = allocate(&a, 16);
		void *test2 = allocate(&a, 16);
		void *test3 = allocate(&a, 16);

		printf("\n%zu, head : %p\n", a.totalsize, a.head);
		printf("must have one block: ");
		curr = a.head;
		while (curr != NULL) {
			printf("block\t");
			curr = curr->next;
		}

		deallocate(&a, &test3);
		printf("\n%zu, head : %p\n", a.totalsize, a.head);
		printf("must have one block: ");
		curr = a.head;
		while (curr != NULL) {
			printf("block\t");
			curr = curr->next;
		}

		deallocate(&a, &test1);
		printf("\n%zu, head : %p\n", a.totalsize, a.head);
		printf("must have two blocks: ");
		curr = a.head;
		while (curr != NULL) {
			printf("block\t");
			curr = curr->next;
		}

		deallocate(&a, &test2);
		printf("\n%zu, head : %p\n", a.totalsize, a.head);
		printf("must have one block: ");
		curr = a.head;
		while (curr != NULL) {
			printf("block\t");
			curr = curr->next;
		}
		printf("\n");
	*/
	return 0;
}
