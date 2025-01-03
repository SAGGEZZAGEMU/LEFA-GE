/* gl_manager.c : OpenGL Values and Functions manager
2023-2024 SILD Team */

#include<stdio.h>

#define _OPENGL
#define GL_VERSION_LIMIT
#define GL2_0
#include<render/api_manager/gl_manager.h>


void loadGladLib()
{
    /*Load GLAD */
    if(!gladLoadGL()) 
    {
        printf("\n \n Error: could start GLAD \n");
    }
}

#if defined(_WIN32)
    #undef APIENTRY

    #include<windows.h>

    //typedef BOOL(APIENTRYP PFNWGLSWAPINTERVALEXTPROC)(int interval);
    //GLAPI PFNWGLSWAPINTERVALEXTPROC glad_wglSwapIntervalEXT;
//    #define wglSwapIntervalEXT glad_wglSwapIntervalEXT

    typedef BOOL(WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int);
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;

    HGLRC hGLRC; /* OpenGL Rendering Context */
    HDC hDC;     /* Device Context */

    void GL_Init(LE_Window *window)
    {

        PIXELFORMATDESCRIPTOR pfd;
        int pixelFormat;

        /* Configurações do formato de pixel */
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;
        pfd.cRedBits = 0;
        pfd.cRedShift = 0;
        pfd.cGreenBits = 0;
        pfd.cGreenShift = 0;
        pfd.cBlueBits = 0;
        pfd.cBlueShift = 0;
        pfd.cAlphaBits = 0;
        pfd.cAlphaShift = 0;
        pfd.cAccumBits = 0;
        pfd.cDepthBits = 32;
        pfd.cStencilBits = 0;
        pfd.cAuxBuffers = 0;
        pfd.iLayerType = PFD_MAIN_PLANE;
        pfd.bReserved = 0;
        pfd.dwLayerMask = 0;
        pfd.dwVisibleMask = 0;
        pfd.dwDamageMask = 0;

        hDC = GetDC((HWND)window->platformHandle);

        pixelFormat = ChoosePixelFormat(hDC, &pfd);
        if (pixelFormat == 0) 
        {
            MessageBox(NULL, "Erro ao escolher o formato de pixel!", "Erro", MB_ICONERROR);
            exit(1);
        }

        if (!SetPixelFormat(hDC, pixelFormat, &pfd)) 
        {
            MessageBox(NULL, "Erro ao configurar o formato de pixel!", "Erro", MB_ICONERROR);
            exit(1);
        }

        /* Cria o contexto de renderização OpenGL */
        hGLRC = wglCreateContext(hDC);
        if (!hGLRC) 
        {
            MessageBox(NULL, "Erro ao criar o contexto OpenGL!", "Erro", MB_ICONERROR);
            exit(1);
        }

        /* Ativa o contexto OpenGL */
        if(!wglMakeCurrent(hDC, hGLRC)) 
        {
            MessageBox(NULL, "Erro ao ativar o contexto OpenGL!", "Erro", MB_ICONERROR);
            exit(1);
        }
        #undef APIENTRY
        loadGladLib();

    }

    void GL_Update(LE_Window *window)
    {
        SwapBuffers(hDC);
    }

    void GL_SetVSync(int interval) 
    {
        if(wglGetProcAddress("wglSwapIntervalEXT")) 
        {
            //printf("Suporte ao wglSwapIntervalEXT encontrado.\n");
        } 
        else 
        {
            printf("Sem suporte ao wglSwapIntervalEXT.\n");
        }

        if(wglSwapIntervalEXT == NULL) 
        {
            wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
        }
        if(wglSwapIntervalEXT) 
        {
            wglSwapIntervalEXT(interval); /* interval = 0 turn off vsync. i hate vsync */
        }
    }

    void GL_Cleanup(LE_Window *window)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hGLRC);
        ReleaseDC((HWND)window->platformHandle, hDC);
    }

#endif /* _WIN32 */

#if defined(__linux__)

    #include<GL/gl.h>
    #include<GL/glx.h>
    #include<GL/glu.h>

    GLXContext glContext;

    void GL_Init(LE_Window *window)
    {
	    XVisualInfo *visualInfo;
        GLint attributes[5];

        attributes[0] = GLX_RGBA;
        attributes[1] = GLX_DEPTH_SIZE;
        attributes[2] = 24;
        attributes[3] = GLX_DOUBLEBUFFER;
        attributes[4] = None;

        visualInfo = glXChooseVisual((Display *)window->platformConnection, 0, attributes);
        if (visualInfo == NULL) 
        {
            printf("No appropriate visual found\n");
        }
        glContext = glXCreateContext((Display *)window->platformConnection, visualInfo, NULL, GL_TRUE);
        glXMakeCurrent((Display *)window->platformConnection, (Window)window->platformHandle, glContext);
    
    loadGladLib();
    }

    void GL_Update(LE_Window *window)
    {
	   glXSwapBuffers((Display *)window->platformConnection, (Window)window->platformHandle);
    }

    void GL_Cleanup(LE_Window *window)
    {
	   glXDestroyContext((Display *)window->platformConnection, glContext);
    }
#endif /* __linux__ */
