#ifndef SGUI_ERROR_H_
#define SGUI_ERROR_H_

/*
 * Usage pattern:
 *
 * SguiError error = {0};
 *
 * if (!fallibleFunction1(someParam, &error))
 *     handleFailure();
 *
 * sguiClearError(&error);
 *
 * if (!fallibleFunction2(anotherParam, &error))
 *     handleFailure();
 *
 * sguiClearError(&error);
 *
 * // so on and so forth ...
 */

typedef enum
{
    SGUI_ERROR_CODE_OK = 0,
    SGUI_ERROR_CODE_OUT_OF_MEMORY,
    SGUI_ERROR_CODE_FORMAT_FAILED,

} SguiErrorCode;

typedef struct
{
    SguiErrorCode code;
    char *message;

} SguiError;

void sguiErrorSet(SguiError *error, SguiErrorCode code, const char *message);
void sguiErrorSetFormatted(SguiError *error, SguiErrorCode code, const char *format, ...);
void sguiErrorClear(SguiError *error);

#define sguiIsOk(error) ((error).code == SGUI_ERROR_CODE_OK)

#endif // SGUI_ERROR_H_
