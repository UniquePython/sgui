#include "sgui/error.h"

#include "private/memory/memory.h"
#include "private/utils/string.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

static void sguiErrorSetMessage(SguiError *error, SguiErrorCode code, char *message)
{
    sguiErrorClear(error);

    error->code = code;
    error->message = message;
}

void sguiErrorSet(SguiError *error, SguiErrorCode code, const char *message)
{
    if (error == NULL)
        return;

    if (message == NULL)
        message = "";

    char *duplicate;

    if (!sguiDuplicateCstr(message, &duplicate))
    {
        sguiErrorSetMessage(error, SGUI_ERROR_CODE_OUT_OF_MEMORY, NULL);
        return;
    }

    sguiErrorSetMessage(error, code, duplicate);
}

void sguiErrorSetFormatted(SguiError *error, SguiErrorCode code, const char *format, ...)
{
    if (error == NULL || format == NULL)
        return;

    va_list args;

    va_start(args, format);
    int lengthRaw = vsnprintf(NULL, 0, format, args);
    va_end(args);

    if (lengthRaw < 0)
    {
        sguiErrorSetMessage(error, SGUI_ERROR_CODE_FORMAT_FAILED, NULL);
        return;
    }

    size_t length = (size_t)lengthRaw + 1;

    char *message;

    if (!sguiAllocateBytes(&message, length))
    {
        sguiErrorSetMessage(error, SGUI_ERROR_CODE_OUT_OF_MEMORY, NULL);
        return;
    }

    va_start(args, format);
    vsnprintf(message, length, format, args);
    va_end(args);

    sguiErrorSetMessage(error, code, message);
}

void sguiErrorClear(SguiError *error)
{
    if (error == NULL)
        return;

    sguiRelease(&error->message);
    error->code = SGUI_ERROR_CODE_OK;
}
