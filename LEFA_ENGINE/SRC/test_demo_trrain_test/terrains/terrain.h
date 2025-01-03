#ifndef TERRAIN_H
#define TERRAIN_H


#include <stdlib.h>
#include <stdio.h>
#include<stdint.h>
#include <math.h>

#include"../models/RawModel.h"
#include"../textures/TerrainTexture.h"
#include"../textures/TerrainTexturePack.h"
#include"../renderEngine/Loader.h"
#include"../toolbox/Maths.h"
#include"../toolbox/DynamicArray.h"
#include"HeightsGenerator.h"

#define SIZE 800 // Tamanho do terreno
#define MAX_HEIGHT 150 // Altura máxima do terreno


float y;
float z;


typedef struct 
{
	Vector3 position;
	RawModel* model;
	TerrainTexturePack* texturePack;
	TerrainTexture* blendMap;
   	
    GLfloat **heights;  // Matriz de alturas

    int heightsLength;
} Terrain;

GLfloat getHeight(Terrain* terrain, int x, int z, HeightsGenerator* generator);

RawModel* generateTerrain(Terrain *terrain, const char *heightMap);


Vector3 calculateNormal(Terrain* terrain,
                        int x, 
                        int z, 
                        HeightsGenerator *generator);



void Terrain_init(Terrain *terrain, 
                  int gridX, 
                  int gridZ, 
                  void *loader, 
                  TerrainTexturePack *texturePack, 
                  TerrainTexture *blendMap, 
                  const char *heightMap) 
{
    uint64_t t1, t2;

    if(!terrain) 
    {
        printf("\n Terrain_init ");
        printf("Erro: the pointer (terrain) is NULL! \n");
    }

    // Inicializar os membros
    terrain->position.x = gridX * SIZE;
    terrain->position.y = 0.0f;
    terrain->position.z = gridZ * SIZE;
    terrain->texturePack = texturePack;
    terrain->blendMap = blendMap;

    // Medir o tempo de geração do terreno
    t1 = Utils_GetMicroSeconds();
    terrain->model = generateTerrain(terrain, heightMap);
    t2 = Utils_GetMicroSeconds();

    // Exibir tempo de execução
    printf("Terrain_init: generateTerrain took %llu microseconds\n", t2 - t1); 
}


float getHeightOfTerrain(Terrain *terrain, float worldX, float worldZ) 
{
    float terrainX = worldX - terrain->position.x;
    float terrainZ = worldZ - terrain->position.z;
    float gridSquareSize = SIZE / ((float)terrain->heightsLength - 1);
    float xCoord, zCoord;
    int gridX = (int)floor(terrainX / gridSquareSize);
    int gridZ = (int)floor(terrainZ / gridSquareSize);

    // Definir as structs corretamente
    Vec3 topLeft, topRight, bottomLeft, bottomRight;
    Vec2 pos;

    // Verifica se está fora dos limites
    if (gridX >= terrain->heightsLength - 1 || 
        gridZ >= terrain->heightsLength - 1 || 
        gridX < 0 || gridZ < 0) 
    {
        return 0.0f;
    }

    xCoord = fmod(terrainX, gridSquareSize) / gridSquareSize;
    zCoord = fmod(terrainZ, gridSquareSize) / gridSquareSize;

    // Coordenadas dos vértices do triângulo
    topLeft.x = 0.0f;
    topLeft.y = terrain->heights[gridX][gridZ];
    topLeft.z = 0.0f;

    topRight.x = 1.0f;
    topRight.y = terrain->heights[gridX + 1][gridZ];
    topRight.z = 0.0f;

    bottomLeft.x = 0.0f;
    bottomLeft.y = terrain->heights[gridX][gridZ + 1];
    bottomLeft.z = 1.0f;

    bottomRight.x = 1.0f;
    bottomRight.y = terrain->heights[gridX + 1][gridZ + 1];
    bottomRight.z = 1.0f;

    pos.x = xCoord;
    pos.y = zCoord;

    // Cálculo da altura usando interpolação baricêntrica
    if (xCoord <= (1 - zCoord)) 
    {
        return barycentric(&topLeft, &topRight, &bottomLeft, &pos);
    } 
    else 
    {
        return barycentric(&topRight, &bottomRight, &bottomLeft, &pos);
    }
}

/*
RawModel* generateTerrain(Terrain *terrain,
                          int stepSize, 
						  const char* heightMap) 
{
	int i, j, gz, gx;
	int vertexIndex = 0;
    int indexIndex = 0;
    int vertexCount = 256 / stepSize; // Calculando o número de vértices
    
    int textureCoordCount = vertexCount * vertexCount * 2; // Duas coordenadas por vértice
    int normalCount = vertexCount * vertexCount * 3;      // Três coordenadas por vértice


    HeightsGenerator generator;
    RawModel* model;

    GLfloat* verticesArray;
    GLfloat* normalsArray;
    GLfloat* textureArray;
    GLuint* indicesArray;
    GLfloat height;

    Vector3 normal;

    if(!terrain) 
    {
        printf("\n generateTerrain ");
        printf("Erro: the pointer (terrain) is NULL! \n");
        return NULL;
    }

    //terrain->heightsLength = vertexCount;

/*    
    // Alocando memória dinamicamente para o array de alturas
    terrain->heights = (GLfloat**)malloc(vertexCount * sizeof(GLfloat*));
    
    for (i = 0; i < vertexCount; i++) 
    {
        terrain->heights[i] = (GLfloat*)malloc(vertexCount * sizeof(GLfloat));
    }

    // Arrays de vértices, normais e textura (com alocação dinâmica)
    verticesArray = (GLfloat*)malloc(vertexCount * vertexCount * 3 * sizeof(GLfloat));
    normalsArray = (GLfloat*)malloc(vertexCount * vertexCount * 3 * sizeof(GLfloat));
    textureArray = (GLfloat*)malloc(vertexCount * vertexCount * 2 * sizeof(GLfloat));
    indicesArray = (GLuint*)malloc((vertexCount - 1) * (vertexCount - 1) * 6 * sizeof(GLuint));

    // Preenchendo os arrays com os dados
    for(i = 0; i < vertexCount; i++) 
    {
        for(j = 0; j < vertexCount; j++) 
        {
            GLfloat s = ((GLfloat)j) / ((GLfloat)(vertexCount - 1));
            GLfloat t = ((GLfloat)i) / ((GLfloat)(vertexCount - 1));

            // Calculando a posição do vértice
            verticesArray[vertexIndex++] = s * SIZE;
            height = getHeight(terrain, j, i, &generator);
            terrain->heights[j][i] = height;
            verticesArray[vertexIndex++] = height;
            verticesArray[vertexIndex++] = t * SIZE;

            // Calculando as normais
            normal = calculateNormal(terrain, j, i, &generator);
            normalsArray[vertexIndex - 3] = normal.x;
            normalsArray[vertexIndex - 2] = normal.y;
            normalsArray[vertexIndex - 1] = normal.z;

            // Coordenadas de textura
            textureArray[(i * vertexCount + j) * 2] = s;
            textureArray[(i * vertexCount + j) * 2 + 1] = t;
        }
    }

    // Gerando índices para o modelo
    for(gz = 0; gz < vertexCount - 1; gz++) 
    {
        for(gx = 0; gx < vertexCount - 1; gx++) 
        {
            GLuint topLeft = (gz * vertexCount) + gx;
            GLuint topRight = topLeft + 1;
            GLuint bottomLeft = ((gz + 1) * vertexCount) + gx;
            GLuint bottomRight = bottomLeft + 1;

            indicesArray[indexIndex++] = topLeft;
            indicesArray[indexIndex++] = bottomLeft;
            indicesArray[indexIndex++] = topRight;
            indicesArray[indexIndex++] = topRight;
            indicesArray[indexIndex++] = bottomLeft;
            indicesArray[indexIndex++] = bottomRight;
        }
    }

    // Carregando o modelo e retornando
    
    model = loadToVAO_noTangents(verticesArray, 
                                 vertexCount * vertexCount,
                                 textureArray, 
                                 textureCoordCount, // não tem esta variavel
                                 normalsArray, 
                                 normalCount,   // não tem esta variavel
                                 indicesArray, 
                                 indexIndex);

    // Liberando memória
    free(verticesArray);
    free(normalsArray);
    free(textureArray);
    free(indicesArray);

    for(i = 0; i < vertexCount; i++) 
    {
        free(terrain->heights[i]);
    }
    free(terrain->heights);
*
    return model;
}
*/

RawModel* generateTerrain(Terrain *terrain, const char *heightMap) 
{
    int stepSize = 4;
    int i, j;
    int idxVert = 0, 
        idxNorm = 0, 
        idxTex  = 0, 
        idxInd  = 0;
    
    int index;
    GLfloat simulatedHeight;

    HeightsGenerator* generator;
    Vector3 normal;
    DynamicArray verticesArray;
    DynamicArray heightsArray;

    int vertexCount;

    GLfloat //*verticesArray, 
            *normalsArray,
            *textureArray;
    
    GLuint  *indicesArray;
    
    GLfloat height, s, t;

    GLfloat ss;

    int returnValue = 0;

    HeightsGenerator_init(generator, heightMap, stepSize);

    vertexCount = getVertexCount(generator);

    // Aloca o array 2D para armazenar as alturas
    terrain->heights = (GLfloat**)malloc(vertexCount * sizeof(GLfloat*));
    for(i = 0; i < vertexCount; i++) 
    {
        terrain->heights[i] = (GLfloat*)malloc(vertexCount * sizeof(GLfloat));
    }
    terrain->heightsLength = vertexCount;

    // Arrays para armazenar dados de vértices, normais, texturas e índices
    //verticesArray = (GLfloat *)malloc(vertexCount * vertexCount * 3 * sizeof(GLfloat));
    normalsArray  = (GLfloat *)malloc(vertexCount * vertexCount * 3 * sizeof(GLfloat));
    textureArray  = (GLfloat *)malloc(vertexCount * vertexCount * 2 * sizeof(GLfloat));
    indicesArray  = (GLuint *)malloc((vertexCount - 1) * (vertexCount - 1) * 6 * sizeof(GLuint));
    
    initDynamicArray(&verticesArray, 64, sizeof(GLfloat));
    initDynamicArray(&heightsArray, 64, sizeof(GLfloat));

    for(i = 0; i < vertexCount; i++) 
    {
        for(j = 0; j < vertexCount; j++) 
        {
            
            s = ((GLfloat)j) / ((GLfloat)(vertexCount - 1));
            t = ((GLfloat)i) / ((GLfloat)(vertexCount - 1));

            ss = s * SIZE;

            // Adiciona as coordenadas dos vértices
            //verticesArray[idxVert++] = s * SIZE;
                pushBack(&verticesArray, &ss);
            //height = getHeight(terrain, j, i, generator);
                height = generateHeight(generator, j, i);
                printf("Height at (%d, %d): %f\n", j, i, height);

            //terrain->heights[j][i] = height;
            /*verticesArray[idxVert++] = height;
            verticesArray[idxVert++] = t * SIZE;

            // Calcula as normais
            //normal = calculateNormal(terrain, j, i, generator);
            /*
            normal = normalize(normal); // Normaliza o vetor
            normalsArray[idxNorm++] = normal.x;
            normalsArray[idxNorm++] = normal.y;
            normalsArray[idxNorm++] = normal.z;

            // Adiciona as coordenadas de textura
            textureArray[idxTex++] = s;
            textureArray[idxTex++] = t;*/
        }
    }

/*
    // Preenche o array de índices
    for (i = 0; i < vertexCount - 1; i++) 
    {
        for (j = 0; j < vertexCount - 1; j++) 
        {
            GLuint topLeft = (i * vertexCount) + j;
            GLuint topRight = topLeft + 1;
            GLuint bottomLeft = ((i + 1) * vertexCount) + j;
            GLuint bottomRight = bottomLeft + 1;

            // Adiciona os índices para formar os triângulos
            indicesArray[idxInd++] = topLeft;
            indicesArray[idxInd++] = bottomLeft;
            indicesArray[idxInd++] = topRight;
            indicesArray[idxInd++] = topRight;
            indicesArray[idxInd++] = bottomLeft;
            indicesArray[idxInd++] = bottomRight;
        }
    }

    getInfo(generator);

    returnValue = loadToVAO_noTangents(verticesArray, 
                                       vertexCount * vertexCount * 3,
                                       textureArray, 
                                       vertexCount * vertexCount * 2,
                                       normalsArray, 
                                       vertexCount * vertexCount * 3,
                                       indicesArray, 
                                       (vertexCount - 1) * (vertexCount - 1) * 6);
*/
    freeDynamicArray(&verticesArray);

    // Retorna o modelo carregado
    //return loader->loadToVAO(verticesArray, textureArray, normalsArray, indicesArray);
    return returnValue;

}




Vector3 calculateNormal(Terrain* terrain,
                        int x, 
                        int z, 
                        HeightsGenerator *generator) 
{
    int stepSize =  getStepSize(generator); // Acessando função membro
    float heightL = getHeight(terrain, x - 1, z, generator);
    float heightR = getHeight(terrain, x + 1, z, generator);
    float heightD = getHeight(terrain, x, z - 1, generator);
    float heightU = getHeight(terrain, x, z + 1, generator);

    // Criando o vetor normal
    Vector3 normal = createVector(heightL - heightR, 2.0f * stepSize, heightD - heightU);
    
    // Normalizando o vetor
    normal = normalize(normal);
    return normal;
}


GLfloat getHeight(Terrain* terrain, 
                  int x, 
                  int z, 
                  HeightsGenerator* generator)
{
    // Chama a função de gerar altura usando o gerador
    return generateHeight(generator, x, z);
}



#endif /* TERRAIN_H */
