

#include<stdio.h>
#include<stdlib.h>


#define _OPENGL
#define GL_VERSION_LIMIT
#define GL2_0
#include<loadEngine.h>

#undef APIENTRY
#include"model_test/ANOTHER/include/pthread/pthread.h"

int winWidth,
    winHeight;

LE_Window* window;


int activeLoop = 1;

RenderInterface* engineRender = NULL;

void* threadFunction(void* arg)
{
    while (activeLoop) 
    {
        printf("Thread: Executando em paralelo ao loop principal!\n");
        sleep(1);  // Pausa por 1 segundo para simular trabalho
    }
    return NULL;
}

void Render() 
{
    /* Limpa a tela e o buffer de profundidade */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glClearColor(0.0f, 0.05f, 0.07f, 1.0f);

    /* Desenha um triângulo simples */
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f, -0.5f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.5f, -0.5f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.5f, 0.0f);
    glEnd();


}

void programLoop()
{
    engineRender->update(window);


	Render();
	getWindowSize(window, &winWidth, &winHeight);
	glViewport(0, 0, winWidth, winHeight);
}

int main(int argc, char **argv, char* envp[])
{
    char renderAPI = 0;
    pthread_t thread_id1, thread_id2;

	printf("!\n Hello World ! \n");


	window = engineCreateDisplay("Lefa Test DEMO !", 800, 600);
    if(!window)
    {
        printf("\n problem in MAIN no window created. \n ");
    }


    renderAPI = 1;

    if(renderAPI == 1)
    {
        engineRender = &openglInterface;
    }
    else if(renderAPI == 2)
    {
        /*engineRender = &vulkanInterface;*/
    };

    engineRender->init(window);

    if(pthread_create(&thread_id1, NULL, threadFunction, NULL) != 0)
    {
        printf("Erro ao criar thread 1 !\n");
        return 1;
    }

    if(pthread_create(&thread_id2, NULL, threadFunction, NULL) != 0)
    {
        printf("Erro ao criar thread 2 !\n");
        return 1;
    }

	engineSetLoop(window, activeLoop, programLoop);

    /* Aguarda a finalização da thread */
    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);

    engineRender->cleanup(window);

return 0;
}
