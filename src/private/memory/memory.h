#ifndef PRIVATE_MEMORY_MEMORY_H_
#define PRIVATE_MEMORY_MEMORY_H_

#include <stdbool.h>
#include <stddef.h>

bool sguiAllocateImpl(size_t size, void **out);
bool sguiAllocateArrayImpl(size_t elementSize, size_t count, void **out);
void sguiReleaseImpl(void **ptr);

#define sguiAllocate(ptr) sguiAllocateImpl(sizeof(*(ptr)), (void **)(ptr))
#define sguiAllocateArray(ptr, count) sguiAllocateArrayImpl(sizeof(*(ptr)), (count), (void **)(ptr))
#define sguiAllocateBytes(ptr, size) sguiAllocateImpl((size), (void **)(ptr))
#define sguiRelease(ptr) sguiReleaseImpl((void **)(ptr))

#endif // PRIVATE_MEMORY_MEMORY_H_
