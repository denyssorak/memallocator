#include "../include/allocator.h"
#include "../include/freeblock.h"
#include <stdio.h>

int main(void) {
	Allocator a;
	initAllocator(&a, 64);

	void *test = allocate(&a, 16);
	void *test2 = allocate(&a, 16);
	printf("%p\n", test);
	printf("%p\n", test2);
	printf("%p\n", a.head->next);

	deallocate(&a, &test);
	printf("%p\n", test);
	printf("%p\n", a.head->next);

	return 0;
}
