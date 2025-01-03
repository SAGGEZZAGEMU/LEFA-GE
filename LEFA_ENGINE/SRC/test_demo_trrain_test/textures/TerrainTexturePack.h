#ifndef TERRAINTEXTUREPACK_H
#define TERRAINTEXTUREPACK_H

#include <stdio.h>

typedef struct {
    GLuint backgroundTexture;
    GLuint rTexture;
    GLuint gTexture;
    GLuint bTexture;
} TerrainTexturePack;

// Função para criar e inicializar um TerrainTexturePack
TerrainTexturePack createTerrainTexturePack(const char* backgroundFile, 
                                            const char* rFile, 
                                            const char* gFile, 
                                            const char* bFile) 
{
    TerrainTexturePack pack;
    
    // Carregar as texturas
    pack.backgroundTexture = loadTexture(backgroundFile);
    pack.rTexture = loadTexture(rFile);
    pack.gTexture = loadTexture(gFile);
    pack.bTexture = loadTexture(bFile);
    
    return pack;
}

// Função para acessar as texturas
GLuint getBackgroundTexture(TerrainTexturePack* pack) 
{
    return pack->backgroundTexture;
}

GLuint getRTexture(TerrainTexturePack* pack) 
{
    return pack->rTexture;
}

GLuint getGTexture(TerrainTexturePack* pack) 
{
    return pack->gTexture;
}

GLuint getBTexture(TerrainTexturePack* pack) 
{
    return pack->bTexture;
}

/*
int main() {
    // Criação do TerrainTexturePack com arquivos de textura
    TerrainTexturePack texturePack = createTerrainTexturePack("res/grassy2", "res/red", "res/green", "res/blue");

    // Usando as texturas
    printf("Background Texture ID: %u\n", getBackgroundTexture(&texturePack));
    printf("Red Texture ID: %u\n", getRTexture(&texturePack));
    printf("Green Texture ID: %u\n", getGTexture(&texturePack));
    printf("Blue Texture ID: %u\n", getBTexture(&texturePack));

    return 0;
}
*/

#endif /* TERRAINTEXTUREPACK_H */

