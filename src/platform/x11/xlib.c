#include "sgui/platform.h"
#include "private/memory/memory.h"

#include <X11/Xlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>

struct SguiWindow
{
    Display *display;
    Window window;
    int screen;

    Atom wmDeleteWindow;

    EventHandlers handlers;

    bool shouldStop;
};

/* ---- createWindow ------------------------------------------------- */

static bool xlibCreateWindow(SguiWindow *window, size_t width, size_t height, const char *title, SguiError *error)
{
    if (window == NULL)
    {
        sguiErrorSet(error, SGUI_ERROR_CODE_NULL_PARAMETER, "createWindow() called with NULL window");
        return false;
    }

    Display *display = XOpenDisplay(NULL);
    if (display == NULL)
    {
        sguiErrorSet(error, SGUI_ERROR_CODE_FAILURE_TO_OPEN_DISPLAY, "createWindow() failed to open display");
        return false;
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    Window win = XCreateSimpleWindow(
        display, root,
        0, 0, (unsigned int)width, (unsigned int)height,
        1,
        BlackPixel(display, screen),
        WhitePixel(display, screen));

    long eventMask = 0;
    XSelectInput(display, win, eventMask);

    if (title)
        XStoreName(display, win, title);

    /* TODO: register for WM_DELETE_WINDOW via XSetWMProtocols so the
     * window manager tells us (via a ClientMessage) when the user
     * clicks the close button, instead of just killing the connection.
     * You'll need XInternAtom(display, "WM_DELETE_WINDOW", False) and
     * store the resulting Atom for comparison in the event loop. */
    Atom wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);

    XMapWindow(display, win);

    window->display = display;
    window->window = win;
    window->screen = screen;
    window->wmDeleteWindow = wmDeleteWindow;
    window->handlers = (EventHandlers){0};
    window->shouldStop = false;

    return true;
}

/* ---- setHandlers ---------------------------------------------------- */

static bool xlibSetHandlers(SguiWindow *window, EventHandlers handlers, SguiError *error)
{
    if (window == NULL)
    {
        sguiErrorSet(error, SGUI_ERROR_CODE_NULL_PARAMETER, "setHandlers() called with NULL window");
        return false;
    }

    window->handlers = handlers;

    return true;
}

/* ---- requestStop ----------------------------------------------------- */

static bool xlibRequestStop(SguiWindow *window, SguiError *error)
{
    if (window == NULL)
    {
        sguiErrorSet(error, SGUI_ERROR_CODE_NULL_PARAMETER, "requestStop() called with NULL window");
        return false;
    }

    window->shouldStop = true;

    return true;
}

/* ---- destroyWindow ----------------------------------------------------- */

static void xlibDestroyWindow(SguiWindow *window)
{
    if (window == NULL)
        return;

    XDestroyWindow(window->display, window->window);
    XCloseDisplay(window->display);
}

/* ---- vtable instance ----------------------------------------------------- */

const PlatformBackend XLibBackend = {
    .createWindow = xlibCreateWindow,
    .setHandlers = xlibSetHandlers,
    .requestStop = xlibRequestStop,
    .destroyWindow = xlibDestroyWindow,
};
