#ifndef SGUI_PLATFORM_H_
#define SGUI_PLATFORM_H_

#include "sgui/error.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct SguiWindow SguiWindow;

/*
 * Callbacks registered by core/app code. The backend is responsible for
 * invoking these at the right time, translating its own native event
 * representation into these calls.
 *
 * Any handler may be NULL — a backend must check before calling.
 */
typedef struct
{
    // Runs when window is closed
    void (*onClose)(SguiWindow *window);
} EventHandlers;

/*
 * The interface every backend must implement identically.
 * Exactly one PlatformBackend instance exists per backend (e.g.
 * XlibBackend), exported as a global const so application code can
 * select it explicitly: `const PlatformBackend *backend = &XlibBackend;`
 *
 * There is no auto-detection here by design — the application decides
 * which backend to use.
 */
typedef struct
{
    /*
     * Create and show a window of the given size/title.
     * Returns NULL on failure (backend should log the reason to stderr).
     */
    bool (*createWindow)(SguiWindow *window, size_t width, size_t height, const char *title, SguiError *error);

    /*
     * Register the callbacks to be invoked during run(). Replaces any
     * previously set handlers. Passing a struct with NULL members
     * disables those specific callbacks.
     */
    bool (*setHandlers)(SguiWindow *window, EventHandlers handlers, SguiError *error);

    /*
     * Signals run() to return after finishing the current iteration.
     * Safe to call from within a callback invoked by run().
     */
    bool (*requestStop)(SguiWindow *window, SguiError *error);

    /*
     * Releases all backend resources associated with window.
     * window must not be used after this call.
     */
    void (*destroyWindow)(SguiWindow *window);
} PlatformBackend;

extern const PlatformBackend XlibBackend;

#endif /* SGUI_PLATFORM_H_ */
