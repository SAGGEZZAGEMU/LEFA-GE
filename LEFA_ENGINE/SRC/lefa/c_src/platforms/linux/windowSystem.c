/* windowsSystem.c : for linux 
2023-2024 SILD Team
*/

#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdarg.h>

#include"core/window_manager.h"

LE_Window* windows[MAX_WINDOWS];
LE_Window *currentWindow = NULL;
int windowCount = 0;


LE_Window* engineCreateDisplay(const char *title, int width, int height)
{
    LE_Window *window = (LE_Window*)malloc(sizeof(LE_Window));

    Display             *display;
    Window               root;
    XSetWindowAttributes attributes;
    int                  screen;
    XColor               color;
    Colormap             colormap;
    int                  screenWidth,
                         screenHeight;


    if(!window) 
    {
        fprintf(stderr, "Erro: não foi possível alocar memória para a estrutura da janela.\n");
        return NULL;
    }

    if (windowCount >= MAX_WINDOWS) 
    {
        printf(" \n ~~[ ALERT! ]: Maximum number of windows reached! Window Limit =  %d \n", windowCount);
        return NULL;
    }

    window->width = width;
    window->height = height;

    /* Open the connection to the X server */
    display = XOpenDisplay(NULL);
    if (display == NULL) 
    {
        printf("\n ~~[ERROR!]: Cannot connect to X server\n\n");
        free(window);
        return NULL;
    }

    screen = DefaultScreen(display);
    root = RootWindow(display, screen);

    colormap = DefaultColormap(display, screen);

    /* Sets the background color */
    if (!XParseColor(display, colormap, "#000700", &color)) 
    {
        fprintf(stderr, "Erro ao analisar a cor.\n");
        XCloseDisplay(display);
        free(window);
        return NULL;
    }

    if (!XAllocColor(display, colormap, &color)) 
    {
        fprintf(stderr, "Erro ao alocar a cor.\n");
        XCloseDisplay(display);
        free(window);
        return NULL;
    }


    /* Window Settings */
    attributes.background_pixel = color.pixel;
    attributes.border_pixel = WhitePixel(display, screen);
    attributes.override_redirect = False;  /* Allows the window manager to control */
    attributes.event_mask = ExposureMask | KeyPressMask;

    /* Adjusting the window position (centering on the screen) */
    screenWidth = DisplayWidth(display, screen);
    screenHeight = DisplayHeight(display, screen);
    window->windowPosX = (screenWidth - width) / 2;
    window->windowPosY = (screenHeight - height) / 2;

    /* Create Window */
    window->platformHandle = (void *)XCreateWindow(display,
                                                   root,
                                                   window->windowPosX,
                                                   window->windowPosY,
                                                   width,
                                                   height,
                                                   0,
                                                   CopyFromParent,
                                                   InputOutput,
                                                   CopyFromParent,
                                                   CWBackPixel | CWBorderPixel | CWEventMask,
                                                   &attributes);

    if (!window->platformHandle) 
    {
        fprintf(stderr, "Erro: não foi possível criar a janela X.\n");
        XCloseDisplay(display);
        free(window);
        return NULL;
    }

    /* Sets the window title */
    XStoreName(display, (Window)window->platformHandle, title);
    XMapWindow(display, (Window)window->platformHandle);
    XFlush(display);

    /* Saves the Display within the structure, in case you need it in the future */
    window->platformConnection = (void *)display;

    windows[windowCount++] = window;
    currentWindow = window;
    return window;
}

void engineDeleteDisplay(LE_Window *window) /* FIXME - Dont Work , destroy first window Only !*/ 
{                                             /* i hate this ! AAAA**/
    Display *display;

    display = (Display *)window->platformConnection;

    if(window) 
    {
        if(window->platformHandle) 
        {
            XDestroyWindow(display, (Window)window->platformHandle);
        }
        free(window);
    }
    else 
    {
        fprintf(stderr, "Warning: Attempted to delete a null window.\n");
    }
}

void getWindowActiveStatus(LE_Window *window) 
{
    Display *display;
    Window focusedWindow;
    int revert;

    /* Open a connection to the X server */
    display = XOpenDisplay(NULL);
    if (display == NULL) 
    {
        /* Failed to open connection to server X */
        window->windowActiveStatus = 0;
        return;
    }

    /* Gets the currently focused window */
    XGetInputFocus(display, &focusedWindow, &revert);

    /* Checks if the current window has focus */
    window->windowActiveStatus = (focusedWindow == (Window)window->platformHandle) ? 1 : 0;

    /* Closes the connection to the X server */
    XCloseDisplay(display);
}

int engineSetLoop(LE_Window *window, int loopVar, void (*renderFunction)())
{
    Display *display;
    XEvent   event;

    display = (Display *)window->platformConnection;


    if(renderFunction == NULL) 
    {
        printf("\n ~~[ERROR!]: [engineSetLoop()] Parameter 3 is NULL!\n");
        return -1;
    }

    while(loopVar)
    {
        while(XPending(display) > 0) 
        {
            XNextEvent(display, &event);

        }
        renderFunction();
    }
    XDestroyWindow(display, (Window)window->platformHandle);
    XCloseDisplay(display);
return 0;
}

void setWindowSize(LE_Window *window, int width, int height) 
{
    Display *display;
    Screen *screen;
    int screenWidth, screenHeight;

    /* Open a connection to the X server */
    display = XOpenDisplay(NULL);
    if(display == NULL) 
    {
        /* Failed to open connection to server X */
        return;
    }

    /* Gets the default screen and its dimensions */
    screen = DefaultScreenOfDisplay(display);
    screenWidth = screen->width;
    screenHeight = screen->height;

    /* Updates the width and height values in the window structure */
    window->width = width;
    window->height = height;

    /* Centers the window on the screen */
    window->windowPosX = (screenWidth - window->width) / 2;
    window->windowPosY = (screenHeight - window->height) / 2;

    if(window && window->platformHandle) 
    {
        /* Move and resize the window */
        XMoveResizeWindow(display, (Window)window->platformHandle, 
                          window->windowPosX, window->windowPosY, 
                          width, height);
    }

    /* Updates the display */
    XFlush(display);

    /* Closes the connection to the X server */
    XCloseDisplay(display);
}

void getWindowSize(LE_Window *window, int *width, int *height) 
{
    Display *display;
    Window win;
    XWindowAttributes attributes;

    /* Gets the display and window from the LE_Window structure */
    display = (Display *)window->platformConnection;
    win = (Window)window->platformHandle;

    /* Gets the window attributes */
    if (XGetWindowAttributes(display, win, &attributes)) 
    {
        *width = attributes.width;
        *height = attributes.height;
    } 
    else 
    {
        printf("Erro: não foi possível obter os atributos da janela.\n");
        *width = 0;
        *height = 0;
    }
}

void engineSetWindowTitle(LE_Window *window, const char *format, ...) 
{
    Display *display;  /* Gets the Display from the rendering context */
    Window win = (Window)window->platformHandle; /* Gets the window from the handle */

    display = (Display *)window->platformConnection;

    char title[256]; /* window title buffer */
    va_list args;

    /* Initialize the argument list */
    va_start(args, format); 
    
    /* Format the title */
    vsnprintf(title, sizeof(title), format, args);

    /* Finalizes the argument list */
    va_end(args);

    /* Sets the window title */
    XStoreName(display, win, title);

    /* Requests the X server to update the screen */
    XFlush(display);
}

void engineSetWindowIcon(LE_Window *window, const char* iconPath)
{
    /* ??? x11 doesn't have a default icon system other than xpm so I'll leave it empty for now */
}


void windowBorderless(LE_Window *window, int mode, ...) /* FIXME */
{
    Display                 *display;
    Window                  win = (Window)window->platformHandle;
    XSetWindowAttributes    attributes;
    XWindowChanges          changes;
    int                     screen = DefaultScreen(display);
    va_list                 args;
    int                     windowWidth = window->width;
    int                     windowHeight = window->height;

    display = (Display *)window->platformConnection;

    va_start(args, mode);

    if (mode == 2) 
    { /* Fullscreen */
        /* Gets the dimensions of the screen */
        Window root = RootWindow(display, screen);
        XWindowAttributes root_attrs;
        XGetWindowAttributes(display, root, &root_attrs);

        windowWidth = root_attrs.width;
        windowHeight = root_attrs.height;

        /* Sets the window to full screen mode */
        attributes.override_redirect = True;
        XChangeWindowAttributes(display, win, CWOverrideRedirect, &attributes);
    } 
    else if (mode == 1) 
    { /* Borderless with specified size */
        windowWidth = va_arg(args, int);
        windowHeight = va_arg(args, int);
        /* Remove borders and decorations */
        attributes.border_pixel = BlackPixel(display, screen);
        XChangeWindowAttributes(display, win, CWBorderPixel, &attributes);
        XSetWindowBorderWidth(display, win, 0);
    } 
    else if (mode == 0) 
    { /* Normal window with borders */
        /* Set border attributes if needed */
        attributes.border_pixel = WhitePixel(display, screen);
        XChangeWindowAttributes(display, win, CWBorderPixel, &attributes);
        XSetWindowBorderWidth(display, win, 1);
    }

    /* Updates window position and size */
    XMoveResizeWindow(display, win, 0, 0, windowWidth, windowHeight);

    /* Map the window to make sure it is visible */
    XMapWindow(display, win);
    XFlush(display);

    va_end(args);
}

