/* window_manager.h : window manipulator system.
2023 - 2024 Sild Team
*/

/*
NOTE !

    This scheme was designed for desktops or window management systems, 
    perhaps it will be implemented in the future for mobile with changes to the function names.
    
    some functions may work differently or have an undesired result on different systems
*/

#ifndef _WINDOW_MANAGER_H_
#define _WINDOW_MANAGER_H_

/* oh my head ! this comment is for me , im so confuse sorry
    windows [HWND] = Linux [Window]
    windows [HINSTANCE] +- Linux [Display]
 */

#define MAX_WINDOWS 14 /* Based on old systems resource limits , 
if you want more windows just #undef this header and increase the limit*/


typedef struct 
{
    void *platformHandle;     /* Platform-specific handle, HWND for Win32, Window for X11, etc. */
    void *platformConnection; /* Platform-specific Instance Server, HINSTANCE for Win32, Display for X11, etc */
    int  width;
    int  height;
    int  windowPosX;
    int  windowPosY;
    int  windowActiveStatus; 
} LE_Window;



LE_Window* engineCreateDisplay(const char *title, int width, int height);

void engineDeleteDisplay(LE_Window *window);

void getWindowActiveStatus(LE_Window *window);

int engineSetLoop(LE_Window *window, int *loopVar, void(*renderFunction)());

void setWindowSize(LE_Window *window, int width, int height);

void getWindowSize(LE_Window *window, int *width, int *height);

void engineSetWindowTitle(LE_Window *window, const char* format, ...);

void engineSetWindowIcon(LE_Window *window, const char* iconPath);

void windowBorderless(LE_Window *window, int mode, ...);





























#endif /*_WINDOW_MANAGER_H_
*/
