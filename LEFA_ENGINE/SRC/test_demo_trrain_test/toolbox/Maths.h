#ifndef MATHS_H
#define MATHS_H

//#include "../Headers.h"
//#include "../entities/Camera.h"


#include <stdio.h>

#define MATRIX_SIZE 4
typedef struct 
{
    float m[MATRIX_SIZE][MATRIX_SIZE];
} Matrix4x4;

typedef struct 
{
    float x, y, z;
}Vector3;

typedef Vector3 Vec3;

typedef struct 
{
    float x, y;
} Vec2;

#define M_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679f

void printMatrix(Matrix4x4* mat, const char* name) 
{
    int i, j;
    
    for (i = 0; i < MATRIX_SIZE; i++) // linhas
    {
        for (j = 0; j < MATRIX_SIZE; j++) // colunas
        {
            printf("%s[%d,%d] = %.5f ", name, i, j, mat->m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

float barycentric(Vec3* p1, 
				  Vec3* p2, 
				  Vec3* p3, 
				  Vec2* pos) 
{
    float det = (p2->z - p3->z) * (p1->x - p3->x) + (p3->x - p2->x) * (p1->z - p3->z);

    // Cálculos das coordenadas baricêntricas l1, l2 e l3
    float l1 = ((p2->z - p3->z) * (pos->x - p3->x) + (p3->x - p2->x) * (pos->y - p3->z)) / det;
    float l2 = ((p3->z - p1->z) * (pos->x - p3->x) + (p1->x - p3->x) * (pos->y - p3->z)) / det;
    float l3 = 1.0f - l1 - l2;

    // Interpolação no eixo Y usando as coordenadas baricêntricas
    return l1 * p1->y + l2 * p2->y + l3 * p3->y;
}

// Função para inicializar a matriz identidade
void initMat4(Matrix4x4* mat) 
{
    int i, j;
    for(i = 0; i < MATRIX_SIZE; i++) 
    {
        for (j = 0; j < MATRIX_SIZE; j++) 
        {
            if (i == j)
            {
                mat->m[i][j] = 1.0f;
            }
            else
            {
                mat->m[i][j] = 0.0f;
            }
        }
    }
}

// Função para aplicar a translação
void translateMat4_vec2(Matrix4x4* mat, Vec2 translation) 
{
    mat->m[0][3] = translation.x;
    mat->m[1][3] = translation.y;
}

void translateMat4_vec3(Matrix4x4* mat, Vec3 translation) 
{
    mat->m[0][3] = translation.x;
    mat->m[1][3] = translation.y;
    mat->m[2][3] = translation.z;  // Inclui a translação no eixo Z
}

// Função para aplicar a escala
void scaleMat4_vec2(Matrix4x4* mat, Vec2 scale) 
{
    mat->m[0][0] = scale.x;
    mat->m[1][1] = scale.y;
}

void scaleMat4_vec3(Matrix4x4* mat, Vec3 scale) 
{
    mat->m[0][0] = scale.x;
    mat->m[1][1] = scale.y;
    mat->m[2][2] = scale.z; // Escala no eixo Z
}

// Função para multiplicar duas matrizes 4x4
void multiplyMat4(Matrix4x4* result, Matrix4x4* a, Matrix4x4* b) 
{
    int i, j, k;
    for (i = 0; i < MATRIX_SIZE; i++) 
    {
        for (j = 0; j < MATRIX_SIZE; j++) 
        {
            result->m[i][j] = 0;
            for (k = 0; k < MATRIX_SIZE; k++) 
            {
                result->m[i][j] += a->m[i][k] * b->m[k][j];
            }
        }
    }
}

// Função para criar uma matriz de rotação em torno de um eixo específico
void rotateMat4(Matrix4x4* mat, float angle, Vec3 axis) 
{
    float rad = angle * (M_PI / 180.0f);  // Converte para radianos
    float cosA = (float)cos(rad);
    float sinA = (float)sin(rad);
    float oneMinusCos = 1.0f - cosA;

    mat->m[0][0] = cosA + axis.x * axis.x * oneMinusCos;
    mat->m[0][1] = axis.x * axis.y * oneMinusCos - axis.z * sinA;
    mat->m[0][2] = axis.x * axis.z * oneMinusCos + axis.y * sinA;
    
    mat->m[1][0] = axis.y * axis.x * oneMinusCos + axis.z * sinA;
    mat->m[1][1] = cosA + axis.y * axis.y * oneMinusCos;
    mat->m[1][2] = axis.y * axis.z * oneMinusCos - axis.x * sinA;
    
    mat->m[2][0] = axis.z * axis.x * oneMinusCos - axis.y * sinA;
    mat->m[2][1] = axis.z * axis.y * oneMinusCos + axis.x * sinA;
    mat->m[2][2] = cosA + axis.z * axis.z * oneMinusCos;

    // A parte de translação não muda na rotação
    mat->m[0][3] = mat->m[1][3] = mat->m[2][3] = 0.0f;
    mat->m[3][3] = 1.0f;
}



// Função para criar a matriz de transformação
Matrix4x4 createTransformationMatrix_2D(Vec2* translation, Vec2* scale) 
{
    Matrix4x4 unit, t, s, result;

    // Inicializa a matriz identidade
    initMat4(&unit);
    
    // Aplica a translação e a escala
    t = unit;
    scaleMat4_vec2(&s, *scale);
    translateMat4_vec2(&t, *translation);
    
    // Multiplica a matriz de translação pela matriz de escala
    multiplyMat4(&result, &t, &s);

    return result;
}


Matrix4x4 createTransformationMatrix_3D(Vec3* translation, 
							 			float rx, 
							 			float ry, 
							 			float rz, 
							 			float scale) 
{
    Matrix4x4 unit, t, rotx, roty, rotz, r, s, result;

    Vec3 axisX, axisY, axisZ;
    Vec2 scaleVec;


    axisX.x = 1.0f;
    axisX.y = 0.0f;
    axisX.z = 0.0f;
    
    axisY.x = 0.0f;
    axisY.y = 1.0f;
    axisY.z = 0.0f;
    
    axisZ.x = 0.0f;
    axisZ.y = 0.0f;
    axisZ.z = 1.0f;

    scaleVec.x = scale; // Inicializa o eixo X com o valor de escala
    scaleVec.y = scale; // Inicializa o eixo Y com o valor de escala

    // Inicializa a matriz identidade
    initMat4(&unit);

    // Aplica a translação
    t = unit;
    translateMat4_vec3(&t, *translation);

    // Aplica a rotação nos três eixos
    rotx = unit;
    rotateMat4(&rotx, rx, axisX);  // Rotação em torno do eixo X
    
    roty = unit;
    rotateMat4(&roty, ry, axisY);  // Rotação em torno do eixo Y

    rotz = unit;
    rotateMat4(&rotz, rz, axisZ);  // Rotação em torno do eixo Z

    // Multiplica as matrizes de rotação
    multiplyMat4(&r, &rotx, &roty);
    multiplyMat4(&r, &r, &rotz);

    // Aplica a escala
    s = unit;
    scaleMat4_vec2(&s, scaleVec);

    // Multiplica translação, rotação e escala
    multiplyMat4(&result, &t, &r);
    multiplyMat4(&result, &result, &s);

    return result;
}

// From OpenGL 3D Game Tutorial 8: Model, View & Projection Matrices
/*
glm::mat4 Maths::createViewMatrix(Camera& camera)
{
	glm::mat4 viewMatrix = glm::mat4(1.0f); // identity matrix

	viewMatrix = glm::rotate(viewMatrix, glm::radians(camera.getPitch()), glm::vec3(1, 0, 0));
	viewMatrix = glm::rotate(viewMatrix, glm::radians(camera.getYaw()), glm::vec3(0, 1, 0));

	glm::vec3 cameraPos = camera.getPosition();
	glm::vec3 negativeCameraPos = glm::vec3(-cameraPos[0], -cameraPos[1], -cameraPos[2]);

	viewMatrix = glm::translate(viewMatrix, negativeCameraPos);

	//printMatrix(viewMatrix, "view");

	return viewMatrix;
}
*/

Vector3 normalize(Vector3 v) 
{
    float length = (float)sqrt((double)(v.x * v.x + v.y * v.y + v.z * v.z));
    if (length > 0.0f) 
    {
        v.x /= length;
        v.y /= length;
        v.z /= length;
    }
    return v;
}

Vector3 createVector(float x, 
                     float y, 
                     float z) 
{
    Vector3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

#endif /* MATHS_H */

