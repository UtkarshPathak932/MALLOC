# MALLOC

A small educational C project that implements a simple allocator over a fixed,
statically allocated heap. It demonstrates tracking allocated and free chunks,
splitting a free chunk to satisfy an allocation, and merging adjacent free
chunks.

## Features

- Fixed-size heap stored in a global byte array.
- First-fit allocation from a sorted list of free chunks.
- Explicit freeing of allocated chunks.
- Coalescing adjacent free chunks before the next allocation.
- Console output showing allocated and free chunks.

## Build and run

You need GCC available in your terminal. From the project directory, run:

```sh
gcc -std=c11 -Wall -Wextra -o heap.exe main.c
```

On Windows PowerShell, run the program with:

```powershell
.\heap.exe
```

On macOS or Linux, you can build and run it with:

```sh
gcc -std=c11 -Wall -Wextra -o heap main.c
./heap
```

The demo in `main` allocates several small chunks, frees some of them, and
prints the resulting allocated and free chunk lists.

## Current status and limitations

This is a learning project, not a drop-in replacement for the C standard
library's `malloc` and `free`.

- `heap_collect` is not implemented.
- `heap_free` expects either `NULL` or a pointer returned by `heap_alloc`;
  invalid pointers trigger an assertion.
- Although `HEAP_CAPACITY` is set to 640,000 bytes, the initial free chunk is
  currently initialized with `CHUNK_LIST_CAP` (1,024) bytes. As a result, the
  allocator currently starts with 1,024 bytes available.
- The chunk lists have a fixed capacity, and allocation failures return `NULL`.

## Project files

- `main.c` — allocator implementation and demonstration program.
