

#include<stdio.h>
#include<stdlib.h>


#define _OPENGL
#define GL_VERSION_LIMIT
#define GL3_0
#include<loadEngine.h>

#include"textures/TerrainTexture.h"
#include"renderEngine/Loader.h"
#include"textures/TerrainTexturePack.h"
#include"terrains/terrain.h"

/* Display Window Size */
int winWidth,
    winHeight;

LE_Window* window; /* Display Window Struct */


int activeLoop = 1; /* Main Program Loop */

RenderInterface* engineRender = NULL;

typedef struct 
{
    unsigned int id;  // ID da textura no OpenGL
    int width, height;
} Texture;

typedef struct 
{
    unsigned int vaoID;    // ID do VAO
    unsigned int vertexCount;
    Texture texture;       // Textura associada
} TexturedModel;

typedef struct {
    TexturedModel model;
    float position[3]; // x, y, z
    float rotX, rotY, rotZ;
    float scale;
} Entity;


Terrain terrainInstance;
Terrain *terrain = &terrainInstance;
Loader* loader;

void Setup()
{
    //******** TERRAIN TEXTURE STUFF ********
    GLuint backgroundTexture = loadTexture("./res/grassy2");
    GLuint rTexture = loadTexture("./res/mud");
    GLuint gTexture = loadTexture("./res/mud");
    GLuint bTexture = loadTexture("./res/mud");


    TerrainTexturePack texturePack = createTerrainTexturePack("./res/grassy2", 
                                                              "./res/red", 
                                                              "./res/green", 
                                                              "./res/blue");
    GLuint blendMap = loadTexture("./res/blendMap");

    Terrain_init(terrain, 
                 0, 
                 -1, 
                 loader, 
                 &texturePack, 
                 (GLuint)blendMap, 
                 "heightmap");

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

	printf("!\n Hello World ! \n");


	window = engineCreateDisplay("Lefa Test DEMO !", 800, 600);
    if(!window)
    {
        printf("\n problem in MAIN no window created. \n ");
    }

    Setup();

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


	engineSetLoop(window, activeLoop, programLoop);



    engineRender->cleanup(window);

return 0;
}
