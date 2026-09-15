# Memory Allocator

Custom memory allocator written from scratch in C.

## Roadmap

 - [x] Create a Memory Free List initialization
 - [x] Allocate Function
 - [x] Deallocate Function (Make sure the allocator reuses memory if possible and doesn't if not possbile)
 - [ ] Create memory merging (merge the memory previously used so it can be reused with greater size)
 - [ ] Reallocate Function (Let the user reallocate memory instead of doing the whole deallocate/allocate cycle)
 - [ ] Callocate for allocating memory for multiple elements of size n bytes each
 - [ ] Perfomance Improvement (After the logic is done, try to improve perfomance speed)
 - [ ] Ensure Thread safety
 - [ ] Implement mmap for large allocations
 - [ ] Create a leak detector
 - [ ] Package as a static/shared library
