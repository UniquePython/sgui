#ifndef SGUI_PLATFORM_H_
#define SGUI_PLATFORM_H_

#include <stddef.h>

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
    // Runs when window is opened
    void (*onOpen)(SguiWindow *window);

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
    SguiWindow *(*createWindow)(size_t width, size_t height, const char *title);

    /*
     * Register the callbacks to be invoked during run(). Replaces any
     * previously set handlers. Passing a struct with NULL members
     * disables those specific callbacks.
     */
    void (*setHandlers)(SguiWindow *window, EventHandlers handlers);

    // Blocks, dispatching registered callbacks, until requestStop() is called.
    void (*run)(SguiWindow *window);

    /*
     * Signals run() to return after finishing the current iteration.
     * Safe to call from within a callback invoked by run().
     */
    void (*requestStop)(SguiWindow *window);

    /*
     * Releases all backend resources associated with window.
     * window must not be used after this call.
     */
    void (*destroyWindow)(SguiWindow *window);
} PlatformBackend;

extern const PlatformBackend XlibBackend;

#endif /* SGUI_PLATFORM_H_ */
