#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include<stdio.h>

#define _OPENGL
#include<loadEngine.h>


LE_Window window;

int activeLoop = 1;

int winWidth, 
	winHeight;

int border = 0;

RenderInterface* renderInterface = NULL;


void initOpenGL() 
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void render() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);

    /* Primeira viewport (cima) */
    glViewport(0, 300, 800, 300);  /* Define a viewport para a metade superior da janela */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(-1.0, -1.0, -1.0);
        glVertex3f(1.0, -1.0, -1.0);
        glVertex3f(0.0, 1.0, -1.0);
    glEnd();

    /* Segunda viewport (baixo) */
    glViewport(0, 0, 800, 300);  /* Define a viewport para a metade inferior da janela */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_TRIANGLES);
        glVertex3f(-1.0, -1.0, -1.0);
        glVertex3f(1.0, -1.0, -1.0);
        glVertex3f(0.0, 1.0, -1.0);
    glEnd();

    glFlush();
}


void renderLoop()
{	
    renderInterface->update(&window);
    render();

	getWindowSize(&window, &winWidth, &winHeight);
	printf("Largura: %d, Altura: %d\n", winWidth, winHeight);

	glViewport(0, 0, winWidth, winHeight);
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)winWidth / (GLfloat)winHeight, 1.0, 100.0);
}

void main()
{
    char renderAPI = 0;

	printf("\n Hello World ! \n ");

	window.width = 800;
	window.height = 600;

	engineCreateDisplay(&window, "Test Linux");

	/*windowBorderless(&window, border, winWidth , winHeight); */
    renderAPI = 1;

    if(renderAPI == 1)
    {
        renderInterface = &openglInterface;
    }
    else if(renderAPI == 2)
    {
        /*renderInterface = &vulkanInterface;*/
    };

    renderInterface->init(&window);

    initOpenGL();

	engineSetLoop(&window, activeLoop, renderLoop);

    renderInterface->cleanup(&window);
}