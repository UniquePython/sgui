#include "private/utils/string.h"
#include "private/memory/memory.h"

#include <stddef.h>
#include <string.h>

bool sguiDuplicateCstr(const char *src, char **dest)
{
    if (src == NULL || dest == NULL)
        return false;

    size_t length = strlen(src) + 1;

    char *duplicate;
    if (!sguiAllocateBytes(&duplicate, length))
        return false;

    memcpy(*duplicate, src, length);

    *dest = duplicate;
    return true;
}
