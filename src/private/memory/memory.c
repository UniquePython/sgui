#include "private/memory/memory.h"

#include <stdlib.h>

bool sguiAllocateImpl(size_t size, void **out)
{
    if (size == 0 || out == NULL)
        return false;

    void *ptr = malloc(size);
    if (ptr == NULL)
        return false;

    *out = ptr;
    return true;
}

bool sguiAllocateArrayImpl(size_t elementSize, size_t count, void **out)
{
    if (elementSize == 0 || count == 0 || out == NULL)
        return false;

    size_t size = elementSize * count;
    if (size < elementSize || size < count) // Check for overflow
        return false;

    return sguiAllocateImpl(size, out);
}

void sguiReleaseImpl(void **ptr)
{
    if (ptr == NULL || *ptr == NULL)
        return;

    free(*ptr);
    *ptr = NULL;
}
