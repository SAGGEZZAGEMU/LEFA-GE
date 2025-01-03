#ifndef TERRAIN_TEXTURE_H
#define TERRAIN_TEXTURE_H

typedef struct 
{
    int textureID;  // Identificador da textura
} TerrainTexture;

// Função para inicializar a estrutura TerrainTexture
static inline 
void TerrainTexture_init(TerrainTexture *texture, int textureID) 
{
    if (texture) 
    {
        texture->textureID = textureID;
    }
}

// Função para obter o ID da textura
static inline 
int TerrainTexture_getID(const TerrainTexture *texture) 
{
    return texture ? texture->textureID : -1; // Retorna -1 se o ponteiro for inválido
}

#endif // TERRAIN_TEXTURE_H

