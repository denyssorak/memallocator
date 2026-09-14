# Memory Allocator

Custom memory allocator written from scratch in C.

## Roadmap

 - [ ] Bump allocator over a static buffer — get the API shape right, no reuse yet
 - [ ] Free list with block headers (size + `next`) stored before user data
 - [ ] Splitting and coalescing free blocks
 - [ ] Real backing memory via `sbrk`/`mmap` instead of a static buffer
 - [ ] Stretch: segregated free lists, arena allocator, or pluggable strategy via function pointers
