#ifndef LOADER_H
#define LOADER_H

#include"3rdparty/glad/OpenGL/glad_gl.h"
#include"3rdparty/lodepng/lodepng.h"
//#include <GL/glew.h>  // Certifique-se de que o GLEW está configurado corretamente
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include"../models/RawModel.h"
#include"../textures/TextureData.h"

typedef struct 
{
    GLuint *vaos;
    GLuint *vbos;
    GLuint *textures;

    int numVaos;        // Número de VAOs
    int numVbos;        // Número de VBOs
    int numTextures;    // Número de texturas
    
    bool useMipMap;
} Loader;

GLuint createVAO();

RawModel* loadToVAO_full(GLfloat* positions, 
                         size_t positionCount,
                         GLfloat* textureCoords, 
                         size_t textureCoordCount,
                         GLfloat* normals, 
                         size_t normalCount,
                         GLfloat* tangents, 
                         size_t tangentCount,
                         GLuint* indices, 
                         size_t indexCount);

RawModel* loadToVAO_noTangents(GLfloat* positions, 
                               size_t positionCount,
                               GLfloat* textureCoords, 
                               size_t textureCoordCount,
                               GLfloat* normals, 
                               size_t normalCount,
                               GLuint* indices, 
                               size_t indexCount);

RawModel* loadToVAO_positionsOnly(GLfloat* positions, 
                                  size_t positionCount, 
                                  int dimensions);

void bindIndicesBuffer(const GLuint *indices, size_t count);
void storeDataInAttributeList(int attributeNumber, 
                              int coordinateSize, 
                              const GLfloat *data, 
                              size_t dataSize);
void unbindVAO();


GLuint loadTexture(const char* fileName) 
{
    unsigned char* image = NULL;
    unsigned width, height;
    
    // Tenta carregar a imagem PNG usando LodePNG
    unsigned error = lodepng_decode32_file(&image, &width, &height, fileName);

    GLuint textureID;

    if(error) 
    {
        printf("Erro ao carregar imagem PNG: %s\n", lodepng_error_text(error));
        return 0;  // Retorna 0 se houve erro
    }

    
    // Gere uma textura OpenGL
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Carrega a imagem na textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    // Gera mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Define parâmetros para o filtro de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Libera a memória da imagem carregada
    free(image);

    // Retorna o ID da textura criada
    return textureID;
}


RawModel* loadToVAO_full(GLfloat* positions, 
                         size_t positionCount,
                         GLfloat* textureCoords, 
                         size_t textureCoordCount,
                         GLfloat* normals, 
                         size_t normalCount,
                         GLfloat* tangents, 
                         size_t tangentCount,
                         GLuint* indices, 
                         size_t indexCount) 
{
    RawModel* model;
    GLuint vaoID = createVAO();

    bindIndicesBuffer(indices, indexCount);
    storeDataInAttributeList(0, 3, positions, positionCount);
    storeDataInAttributeList(1, 2, textureCoords, textureCoordCount);
    storeDataInAttributeList(2, 3, normals, normalCount);
    storeDataInAttributeList(3, 3, tangents, tangentCount);
    unbindVAO();

    model = (RawModel*)malloc(sizeof(RawModel));
    if (model != NULL) 
    {
        model->vaoID = vaoID;
        model->indexCount = indexCount;
    }
    return model;
}


RawModel* loadToVAO_noTangents(GLfloat* positions, 
                               size_t positionCount,
                               GLfloat* textureCoords, 
                               size_t textureCoordCount,
                               GLfloat* normals, 
                               size_t normalCount,
                               GLuint* indices, 
                               size_t indexCount) 
{
    RawModel* model;
    GLuint vaoID = createVAO();

    bindIndicesBuffer(indices, indexCount);
    storeDataInAttributeList(0, 3, positions, positionCount);
    storeDataInAttributeList(1, 2, textureCoords, textureCoordCount);
    storeDataInAttributeList(2, 3, normals, normalCount);
    unbindVAO();

    model = (RawModel*)malloc(sizeof(RawModel));
    if(model != NULL) 
    {
        model->vaoID = vaoID;
        model->indexCount = indexCount;
    }
    return model;
}


RawModel* loadToVAO_positionsOnly(GLfloat* positions, 
                                  size_t positionCount, 
                                  int dimensions) 
{
    RawModel* model;
    GLuint vaoID = createVAO();

    storeDataInAttributeList(0, dimensions, positions, positionCount);
    unbindVAO();

    model = (RawModel*)malloc(sizeof(RawModel));
    if(model != NULL) 
    {
        model->vaoID = vaoID;
        model->indexCount = positionCount / dimensions;
    }
    return model;
}

#define MAX_VAOS 256

    GLuint vaos[MAX_VAOS];
    int vaoCount = 0;

GLuint createVAO() 
{
    GLuint vaoID = 0;


    // Gera um VAO
    glGenVertexArrays(1, &vaoID);

    // Verifica se ainda há espaço no array
    if(vaoCount < MAX_VAOS) 
    {
        vaos[vaoCount++] = vaoID; // Adiciona o VAO ao array e incrementa o contador
    } 
    else 
    {
        // Caso exceda o limite, você pode emitir um erro ou lidar de outra forma
        fprintf(stderr, "Erro: número máximo de VAOs alcançado.\n");
    }

    // Vincula o VAO
    glBindVertexArray(vaoID);

    return vaoID;
}

void unbindVAO() 
{
    glBindVertexArray(0);
}

void storeDataInAttributeList(int attributeNumber, 
                              int coordinateSize, 
                              const GLfloat *data, 
                              size_t dataSize) 
{
    GLuint vboID;

    // Cria um VBO (Vertex Buffer Object)
    glGenBuffers(1, &vboID);

    // Vincula o buffer ao alvo de array
    glBindBuffer(GL_ARRAY_BUFFER, vboID);

    // Passa os dados para o buffer
    glBufferData(GL_ARRAY_BUFFER, dataSize * sizeof(GLfloat), data, GL_STATIC_DRAW);

    // Especifica o layout dos dados no atributo
    glVertexAttribPointer(attributeNumber, coordinateSize, GL_FLOAT, GL_FALSE, 0, (const void*)0);

    // Desvincula o buffer atual
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


#define MAX_INDICES 1024 // Defina o tamanho máximo do array conforme necessário

void bindIndicesBuffer(const GLuint *indices, size_t count) 
{
    GLuint vboID;

    // Cria um VBO (Vertex Buffer Object)
    glGenBuffers(1, &vboID);

    // Vincula o buffer ao alvo de índice
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);

    // Passa os dados para o buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), indices, GL_STATIC_DRAW);
}


GLubyte* LoadPNGImage(const char* imageFile, 
                      GLsizei *width, 
                      GLsizei *height, 
                      GLenum *format) 
{
    unsigned char* image = NULL;  // Buffer de imagem
    unsigned error;               // Código de erro retornado pela lodepng

    // Carrega o arquivo PNG
    error = lodepng_decode32_file(&image, 
                                  (unsigned int*)width, 
                                  (unsigned int*)height, 
                                  imageFile);  // Passando o endereço do ponteiro
    if(error) 
    {
        fprintf(stderr, "Erro ao carregar a imagem PNG: %s\n", lodepng_error_text(error));
        return NULL;
    }

    // Define o formato da imagem
    *format = GL_RGBA;  // O formato da imagem será sempre RGBA após a decodificação

    return image;
}

TextureData* decodeTextureFile(const char* fileName) 
{
    GLsizei width, height;
    GLenum format;
    
    // Chama LoadPNGImage para carregar a imagem
    GLubyte* buffer = LoadPNGImage(fileName, &width, &height, &format);
    if (buffer) 
    {
        // Aloca memória para o TextureData e preenche com os dados da imagem
        TextureData* texture = (TextureData*)malloc(sizeof(TextureData));
        if (texture == NULL) 
        {
            fprintf(stderr, "Erro de alocação de memória: %s\n", strerror(errno));
            exit(1);
        }
        
        texture->buffer = buffer;
        texture->width = width;
        texture->height = height;
        
        return texture;
    }
    
    // Caso falhe ao carregar a imagem, exibe erro e finaliza o programa
    fprintf(stderr, "Loader: Failed to load texture \"%s\"\n", fileName);
    exit(1);
    
    return NULL; // Código redundante, já que o exit termina o programa
}

#endif /* LOADER_H */
