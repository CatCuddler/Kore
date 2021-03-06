#include "../pch.h"

#include <kinc/input/mouse.h>
#include <kinc/log.h>
#include <kinc/system.h>
#include <kinc/window.h>

#include "../WindowData.h"

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/X.h>
#include <X11/keysym.h>
#include <X11/Xcursor/Xcursor.h>

#define MAXIMUM_WINDOWS 16
extern Kore::WindowData kinc_internal_windows[MAXIMUM_WINDOWS];

void kinc_internal_mouse_lock(int window) {
    kinc_mouse_hide();
    int width = kinc_window_width(window);
    int height = kinc_window_height(window);

    int x, y;
    kinc_mouse_get_position(window, &x, &y);

    // Guess the new position of X and Y
    int newX = x;
    int newY = y;

    // Correct the position of the X coordinate
    // if the mouse is out the window
    if (x < 0) {
        newX -= x;
    }
    else if (x > width) {
        newX -= x - width;
    }

    // Correct the position of the Y coordinate
    // if the mouse is out the window
    if (y < 0) {
        newY -= y;
    }
    else if (y > height) {
        newY -= y - height;
    }

    // Force the mouse to stay inside the window
    kinc_mouse_set_position(window, newX, newY);
}

void kinc_internal_mouse_unlock(int window) {
    kinc_mouse_show();
}

bool kinc_mouse_can_lock(int window) {
	return true;
}

bool _mouseHidden = false;

void kinc_mouse_show() {
#ifdef KORE_OPENGL
	::Display* dpy = glXGetCurrentDisplay();
	::Window win = (XID)kinc_internal_windows[0].handle;
	if (_mouseHidden)
    {
		//log(LogLevel::Info, "show mouse\n");
        XUndefineCursor(dpy, win);
		_mouseHidden = false;
    }
#endif
}

void kinc_mouse_hide() {
#ifdef KORE_OPENGL
    ::Display* dpy = glXGetCurrentDisplay();
    ::Window win = (XID)kinc_internal_windows[0].handle;
    if (!_mouseHidden)
    {
        //log(LogLevel::Info, "hide mouse\n");
        XColor col = XColor{0, 0, 0, 0, DoRed | DoGreen | DoBlue, 0};
        char data[1] = {'\0'};
        Pixmap blank = XCreateBitmapFromData(dpy, win, data, 1, 1);
        Cursor cursor = XCreatePixmapCursor(dpy, blank, blank, &col, &col, 0, 0);
        XDefineCursor(dpy, win, cursor);
        XFreePixmap(dpy, blank);
        _mouseHidden = true;
    }
#endif
}

void kinc_mouse_set_cursor(int cursorIndex) {
#ifdef KORE_OPENGL
    ::Display* dpy = glXGetCurrentDisplay();
    ::Window win = (XID)kinc_internal_windows[0].handle;
    if (!_mouseHidden)
    {
        Cursor cursor;
        switch(cursorIndex){
            case 0: {
                cursor = XcursorLibraryLoadCursor(dpy, "arrow");
                break;
            }
            case 1: {
                cursor = XcursorLibraryLoadCursor(dpy, "pointer");
                break;
            }
            case 2: {
                cursor = XcursorLibraryLoadCursor(dpy, "text");
                break;
            }
            case 3: {
                cursor = XcursorLibraryLoadCursor(dpy, "ew-resize");
                break;
            }
            case 4: {
                cursor = XcursorLibraryLoadCursor(dpy, "ns-resize");
                break;
            }
            case 5: {
                cursor = XcursorLibraryLoadCursor(dpy, "ne-resize");
                break;
            }
            case 6: {
                cursor = XcursorLibraryLoadCursor(dpy, "se-resize");
                break;
            }
            case 7: {
                cursor = XcursorLibraryLoadCursor(dpy, "nw-resize");
                break;
            }
            case 8: {
                cursor = XcursorLibraryLoadCursor(dpy, "sw-resize");
                break;
            }
            case 9: {
                cursor = XcursorLibraryLoadCursor(dpy, "grab");
                break;
            }
            case 10: {
                cursor = XcursorLibraryLoadCursor(dpy, "grabbing");
                break;
            }
            case 11: {
                cursor = XcursorLibraryLoadCursor(dpy, "not-allowed");
                break;
            }
            case 12: {
                cursor = XcursorLibraryLoadCursor(dpy, "wait");
                break;
            }
            case 13: {
                cursor = XcursorLibraryLoadCursor(dpy, "crosshair");
                break;
            }
            default: {
                cursor = XcursorLibraryLoadCursor(dpy, "arrow");
                break;
            }
        }
        XDefineCursor(dpy, win, cursor);
    }   
#endif
}

void kinc_mouse_set_position(int window, int x, int y) {
#ifdef KORE_OPENGL
	::Display* dpy = XOpenDisplay(0);
	::Window win = (XID)kinc_internal_windows[0].handle;

	XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
	XFlush(dpy); // Flushes the output buffer, therefore updates the cursor's position.

	XCloseDisplay(dpy);
#endif
}

void kinc_mouse_get_position(int window, int *x, int *y) {
#ifdef KORE_OPENGL
	::Display* dpy = XOpenDisplay(NULL);
	::Window win = (XID)kinc_internal_windows[0].handle;

	::Window inwin;
	::Window inchildwin;
	int rootx, rooty;
	unsigned int mask;

	XQueryPointer(dpy, win, &inwin, &inchildwin, &rootx, &rooty, x, y, &mask);

	XCloseDisplay(dpy);
#endif
}
