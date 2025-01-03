#ifndef HEIGHTSGENERATOR_H
#define HEIGHTSGENERATOR_H

#define MAX_HEIGHT 150 // Altura máxima do terreno

#include"../textures/TextureData.h"
#include"../toolBox/Const.h"
#include"../toolBox/Utils.h"


#define INT_MAX 2147483647
#define INT_MIN (-2147483647 - 1)

typedef struct
{
	TextureData *image;
	int stepSize;
	int vertexCount;
	int imageStepSize;
	int imageVertexCount;

	int seed;
	GLfloat amplitude;
	int octaves;
	double roughness;

	int xmin, xmax, zmin, zmax;
	int getNoiseCalls;
	int getNoise1Calls;
	int getSmoothNoiseCalls;
	int getInterpolatedNoiseCalls;

	GLfloat* noiseTable;
}HeightsGenerator;

GLfloat interpolate(GLfloat a, GLfloat b, GLfloat blend);
GLfloat getNoise(HeightsGenerator* generator, int x, int z);
GLfloat getInterpolatedNoise(HeightsGenerator* generator, GLfloat x, GLfloat z);
GLfloat getSmoothNoise(HeightsGenerator* generator, int x, int z);

float getNoise1(HeightsGenerator *hg, int x, int z);

int getVertexCount(HeightsGenerator *generator) 
{
    return generator->vertexCount;
}


void HeightsGenerator_init(HeightsGenerator *generator, 
						   const char *heightMap, 
						   int stepSize) 
{
    int index = 0;
    int z, x;


    generator->stepSize = stepSize;
    generator->vertexCount = 256 / stepSize;

    printf("vertexCount: %d\n", generator->vertexCount);
    printf("stepSize: %d\n", generator->stepSize);

    generator->image = NULL;

    if(heightMap != NULL && heightMap[0] != '\0') 
    {
        char fileName[256];
        snprintf(fileName, sizeof(fileName), "%s%s.png", RES_LOC, heightMap);
        
        generator->image = decodeTextureFile(fileName);
        if (generator->image == NULL) 
        {
            fprintf(stderr, "HeightsGenerator: decodeTextureFile failed for %s\n", fileName);
            exit(1);
        }

        generator->imageVertexCount = generator->image->height; // Usando 'height' de TextureData
        printf("image vertexCount: %d\n", generator->imageVertexCount);

        generator->imageStepSize = generator->imageVertexCount / generator->vertexCount;
        if(generator->imageStepSize < 1) 
        {
            generator->imageStepSize = 1;
        }
        printf("image stepSize: %d\n", generator->imageStepSize);
    }

    generator->seed = 431;
    generator->amplitude = 70;
    generator->octaves = 4;
    generator->roughness = 0.3f;

    generator->xmin = INT_MAX;
    generator->xmax = INT_MIN;
    generator->zmin = INT_MAX;
    generator->zmax = INT_MIN;
    generator->getNoiseCalls = 0;
    generator->getSmoothNoiseCalls = 0;
    generator->getInterpolatedNoiseCalls = 0;

    // Gerar a tabela de ruído
    generator->noiseTable = (GLfloat *)malloc((generator->vertexCount + 2) * 
                                              (generator->vertexCount + 2) * 
                                                                  stepSize * 
                                                                  stepSize * 
                                                                  sizeof(GLfloat));
    
    for(z = -stepSize; z < generator->vertexCount * stepSize + stepSize; z++) 
    {
        for(x = -stepSize; x < generator->vertexCount * stepSize + stepSize; x++) 
        {
            generator->noiseTable[index++] = getNoise1(generator,x, z);
        }
    }

    printf("size of noise table: %d\n", index);
    printf("should be: %d\n", (generator->vertexCount + 2) * (generator->vertexCount + 2) * stepSize * stepSize);
}

void getInfo(HeightsGenerator *this)
{
    printf("xmin: %f\n", this->xmin);
    printf("xmax: %f\n", this->xmax);
    printf("zmin: %f\n", this->zmin);
    printf("zmax: %f\n", this->zmax);
    printf("getNoise() calls: %d\n", this->getNoiseCalls);
    printf("getNoise1() calls: %d\n", this->getNoise1Calls);
    printf("getSmoothNoise() calls: %d\n", this->getSmoothNoiseCalls);
    printf("getInterpolatedNoise() calls: %d\n", this->getInterpolatedNoiseCalls);

    // Libera memória alocada para noiseTable
    free(this->noiseTable);
}

GLfloat getHeightFromImage(HeightsGenerator* generator, int x, int z)
{
    int index, r;
    GLfloat height;

    /* Verifica se a imagem é nula */
    if (generator->image == NULL) 
    {
        return 0;
    }

    /* Verifica os limites da imagem */
    if(x < 0 || x >= TextureData_getWidth(generator->image) || 
       z < 0 || z >= TextureData_getHeight(generator->image)) 
    {
        return 0;
    }

    /* Calcula o índice do pixel no buffer */
    index = 4 * (x + z * TextureData_getWidth(generator->image));
    r = *(TextureData_getBuffer(generator->image) + index);

    /* Ajusta o valor do componente vermelho (R) */
    height = (GLfloat)r;
    height -= 127.5f;
    height /= 127.5f; /* Deve estar entre -1 e 1 */
    height *= MAX_HEIGHT;

    return height;
}

GLfloat generateHeight(HeightsGenerator* generator, int x, int z)
{
    int imageX = x * generator->imageStepSize;
    int imageZ = z * generator->imageStepSize;
    int i;

    GLfloat heightFromImage;
    GLfloat total = 0.0f;
    GLfloat d;


    x *= generator->stepSize;
    z *= generator->stepSize;

    
    d = (GLfloat) (2 << (generator->octaves));  // Ajuste para octaves

    for(i = 0; i < generator->octaves; i++) 
    {
        GLfloat freq = (GLfloat) (2 << i) / d;
        GLfloat amp = (GLfloat) pow(generator->roughness, i) * generator->amplitude;
        total += getInterpolatedNoise(generator, x * freq, z * freq) * amp;
    }

    heightFromImage = getHeightFromImage(generator, imageX, imageZ);
    total += heightFromImage;

    return total;
}


GLfloat getInterpolatedNoise(HeightsGenerator* generator, GLfloat x, GLfloat z)
{
    // Converte os valores de ponto flutuante para inteiro
    int intX = (int)x;
    int intZ = (int)z;

    GLfloat fracX, fracZ;
    GLfloat v1, v2, v3, v4;
    GLfloat i1, i2;

    // Incrementa o contador de chamadas
    generator->getInterpolatedNoiseCalls++;


    // Calcula as partes fracionárias
    fracX = x - (GLfloat)intX;
    fracZ = z - (GLfloat)intZ;

    // Obtém os valores de ruído suavizados
    v1 = getSmoothNoise(generator, intX, intZ);
    v2 = getSmoothNoise(generator, intX + 1, intZ);
    v3 = getSmoothNoise(generator, intX, intZ + 1);
    v4 = getSmoothNoise(generator, intX + 1, intZ + 1);

    // Interpola os valores
    i1 = interpolate(v1, v2, fracX);
    i2 = interpolate(v3, v4, fracX);

    // Retorna o resultado interpolado final
    return interpolate(i1, i2, fracZ);
}


GLfloat interpolate(GLfloat a, GLfloat b, GLfloat blend)
{
    double theta = blend * M_PI;
    GLfloat f = (GLfloat)((1.0 - cos(theta)) * 0.5);
    return a * (1.0f - f) + b * f;
}

GLfloat getSmoothNoise(HeightsGenerator* generator, int x, int z)
{
    GLfloat corners, sides, center, result;

    /* Incrementa o contador de chamadas na estrutura */
    generator->getSmoothNoiseCalls++;

    /* Calcula as contribuições dos cantos */
    corners = (getNoise(generator, x - 1, z - 1) + 
               getNoise(generator, x + 1, z - 1) + 
               getNoise(generator, x - 1, z + 1) + 
               getNoise(generator, x + 1, z + 1)) / 16.0f;

    /* Calcula as contribuições dos lados */
    sides = (getNoise(generator, x - 1, z) + 
             getNoise(generator, x + 1, z) + 
             getNoise(generator, x, z + 1) + 
             getNoise(generator, x, z - 1)) / 8.0f;

    /* Calcula a contribuição do centro */
    center = getNoise(generator, x, z) / 4.0f;

    /* Soma tudo para obter o valor suavizado */
    result = corners + sides + center;

    return result;
}


GLfloat getNoise(HeightsGenerator* generator, int x, int z)
{
    int valuesPerRow, ind;
    GLfloat value;

    /* Incrementa o contador de chamadas */
    generator->getNoiseCalls++;

    /* Calcula valores auxiliares */
    valuesPerRow = 2 * generator->stepSize + generator->vertexCount * generator->stepSize;
    ind = x + generator->stepSize; /* -stepSize vira 0 */
    ind += (z + generator->stepSize) * valuesPerRow;

    /* Obtém o valor da tabela de ruído */
    value = generator->noiseTable[ind];

    /* Retorna o valor calculado */
    return value;
}

int getStepSize(HeightsGenerator* generator) 
{
    return generator->stepSize;
}


float getNoise1(HeightsGenerator *hg, int x, int z)
{
    float returnValue = Utils_Rand() * 2.0f - 1.0f;

    // Atualiza os limites de x e z
    if(x < hg->xmin) 
    {
        hg->xmin = x;
    }
    
    if(x > hg->xmax) 
    {
        hg->xmax = x;
    }

    if(z < hg->zmin) 
    {
        hg->zmin = z;
    }

    if(z > hg->zmax) 
    {
        hg->zmax = z;
    }

    // Conta as chamadas
    hg->getNoise1Calls++;

    // Gera a semente com base nas coordenadas
    Utils_SeedRand(x * 963 + z * 13251 + hg->seed * 31);

    // Retorna o valor gerado (ruído)
    return returnValue;
}







#endif /* HEIGHTSGENERATOR_H */

