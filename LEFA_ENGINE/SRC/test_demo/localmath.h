#ifndef LOCALMATH_H
#define LOCALMATH_H


#	define HYP_STATIC  /* staic befor function */
#	define HYPATIA_SINGLE_PRECISION_FLOATS /* Use Float / if disabled = double */
#	define HYPATIA_IMPLEMENTATION  /* Load Full */
#	include"3rdparty/hypatia/hypatia.h"


#define mat4x4 struct matrix4 /* Hypatia */
#define Matrix4x4 struct matrix4 /* Hypatia */
#define Vector2 struct vector2 /* Hypatia */
#define Vector3 struct vector3 /* Hypatia */
#define Vector4 struct vector4 /* Hypatia */

#define PI HYP_PI
#define M_PI HYP_PI

static 
float radians(float degrees) 
{
    return degrees * (PI / 180.0f);
}


// Inicializa uma matriz identidade
static 
Matrix4x4 identity_matrix() 
{
    int i = 0;
    Matrix4x4 mat = {0};
    
    for(i = 0; i < 4; i++) 
    {
        mat.m44[i][i] = 1.0f;
    }
    return mat;
}

// Multiplica uma matriz 4x4 por um vetor 3D
static 
Vector3 multiply_matrix_vector(const Matrix4x4 *mat, const Vector3 *vec) 
{
    Vector3 result;

    result.x = mat->m44[0][0] * vec->x + mat->m44[0][1] * vec->y + mat->m44[0][2] * vec->z + mat->m44[0][3];
    result.y = mat->m44[1][0] * vec->x + mat->m44[1][1] * vec->y + mat->m44[1][2] * vec->z + mat->m44[1][3];
    result.z = mat->m44[2][0] * vec->x + mat->m44[2][1] * vec->y + mat->m44[2][2] * vec->z + mat->m44[2][3];
    return result;
}

// Cria uma matriz de translação
static 
Matrix4x4 translation_matrix(float tx, float ty, float tz) 
{
    Matrix4x4 mat = identity_matrix();
    
    mat.m44[0][3] = tx;
    mat.m44[1][3] = ty;
    mat.m44[2][3] = tz;
    return mat;
}

static 
Vector3 subtract(Vector3 a, Vector3 b) 
{
    Vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

// Soma de vetores
static 
Vector3 add(Vector3 a, Vector3 b) 
{
    Vector3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

static 
float dot(Vector3 a, Vector3 b) 
{
    return a.x * b.x + a.y * 
    	   b.y + a.z * b.z;
}

// Produto vetorial (cross product)
static 
Vector3 cross(Vector3 a, Vector3 b) 
{
    Vector3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

// Normalização de vetor
static 
Vector3 normalize(Vector3 v) 
{
    Vector3 result;
    float length = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    
    if (length == 0.0f) 
    {
        Vector3 zero = {0.0f, 0.0f, 0.0f};
        return zero; // Retorna um vetor zero se o comprimento for 0
    }
    result.x = v.x / length;
    result.y = v.y / length;
    result.z = v.z / length;
    return result;
}

// Escalar vetor por um valor
static 
Vector3 scale(Vector3 v, float scalar) 
{
    Vector3 result;
    result.x = v.x * scalar;
    result.y = v.y * scalar;
    result.z = v.z * scalar;
    return result;
}


static 
Matrix4x4 multiply_matrices(const Matrix4x4 *a, const Matrix4x4 *b) 
{
    int i, j, k;

    Matrix4x4 result;
    memset(&result, 0, sizeof(Matrix4x4)); // Inicializa a matriz resultante com zeros

    for(i = 0; i < 4; i++) 
    {
        for(j = 0; j < 4; j++) 
        {
            for(k = 0; k < 4; k++) 
            {
                result.m44[i][j] += a->m44[i][k] * b->m44[k][j];
            }
        }
    }

    return result;
}

// Cria uma matriz de escala
static Matrix4x4 scale_matrix(Matrix4x4 mat, float scale) 
{
    Matrix4x4 result = identity_matrix();
    result.m44[0][0] = scale;
    result.m44[1][1] = scale;
    result.m44[2][2] = scale;

    return multiply_matrices(&mat, &result);
}

static 
Matrix4x4 perspective(float fov, float aspect, float nearf, float farf) 
{
    Matrix4x4 result;
    float tanHalfFov = tan(fov / 2.0f);


    memset(&result, 0, sizeof(Matrix4x4));

    result.m44[0][0] = 1.0f / (aspect * tanHalfFov);
    result.m44[1][1] = 1.0f / tanHalfFov;
    result.m44[2][2] = -(farf + nearf) / (farf - nearf);
    result.m44[2][3] = -1.0f;
    result.m44[3][2] = -(2.0f * farf * nearf) / (farf - nearf);
    result.m44[3][3] = 0.0f;

    return result;
}


// Cria uma matriz de visualização (lookAt)
static 
Matrix4x4 look_at(float eyeX, 
                  float eyeY, 
                  float eyeZ, 
                  float centerX, 
                  float centerY, 
                  float centerZ, 
                  float upX, 
                  float upY, 
                  float upZ) 
{
    float fx,fy,fz,
    len, sx, sy, sz,
    ux, uy, uz;
    
    Matrix4x4 mat;

    fx = centerX - eyeX;
    fy = centerY - eyeY;
    fz = centerZ - eyeZ;
    len = sqrt(fx * fx + fy * fy + fz * fz);

    fx /= len; fy /= len; fz /= len;

    sx = fy * upZ - fz * upY;
    sy = fz * upX - fx * upZ;
    sz = fx * upY - fy * upX;

    len = sqrt(sx * sx + sy * sy + sz * sz);

    sx /= len; sy /= len; sz /= len;

    ux = sy * fz - sz * fy;
    uy = sz * fx - sx * fz;
    uz = sx * fy - sy * fx;

    mat = identity_matrix();
    mat.m44[0][0] = sx; mat.m44[1][0] = sy; mat.m44[2][0] = sz;
    mat.m44[0][1] = ux; mat.m44[1][1] = uy; mat.m44[2][1] = uz;
    mat.m44[0][2] = -fx; mat.m44[1][2] = -fy; mat.m44[2][2] = -fz;
    mat.m44[3][0] = -(sx * eyeX + sy * eyeY + sz * eyeZ);
    mat.m44[3][1] = -(ux * eyeX + uy * eyeY + uz * eyeZ);
    mat.m44[3][2] = fx * eyeX + fy * eyeY + fz * eyeZ;
    return mat;
}

/* // trying hypatia but bad
static 
Matrix4x4 look_at(float eyeX, 
                  float eyeY, 
                  float eyeZ, 
                  float centerX, 
                  float centerY, 
                  float centerZ, 
                  float upX, 
                  float upY, 
                  float upZ) 
{
    Vector3 eye;
    Vector3 target;
    Vector3 up;
    Matrix4x4 viewMatrix;

    eye.x = eyeX;
    eye.y = eyeY;
    eye.z = eyeZ;

    target.x = centerX;
    target.y = centerY;
    target.z = centerZ;

    up.x = upX;
    up.y = upY;
    up.z = upZ;


    matrix4_view_lookat_rh_EXP(&viewMatrix, &eye, &target, &up);

    return *((Matrix4x4 *)&viewMatrix); // Converte a struct para Matrix4x4
}*/

// Cria uma matriz de rotação em torno do eixo X
static 
Matrix4x4 rotate_x(float angle) 
{
    Matrix4x4 mat = identity_matrix();
    mat.m44[1][1] = cos(angle);
    mat.m44[1][2] = -sin(angle);
    mat.m44[2][1] = sin(angle);
    mat.m44[2][2] = cos(angle);
    return mat;
}

// Cria uma matriz de rotação em torno do eixo Y
static 
Matrix4x4 rotate_y(float angle) 
{
    Matrix4x4 mat = identity_matrix();
    mat.m44[0][0] = cos(angle);
    mat.m44[0][2] = sin(angle);
    mat.m44[2][0] = -sin(angle);
    mat.m44[2][2] = cos(angle);
    return mat;
}

// Cria uma matriz de rotação em torno do eixo Z
static 
Matrix4x4 rotate_z(float angle) 
{
    Matrix4x4 mat = identity_matrix();
    mat.m44[0][0] = cos(angle);
    mat.m44[0][1] = -sin(angle);
    mat.m44[1][0] = sin(angle);
    mat.m44[1][1] = cos(angle);
    return mat;
}

static 
Vector3 rotate_vector3(Vector3 vector, Vector3 axis, float angle) 
{
    Vector3 rotated;
    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    // Fórmula de rotação de Rodrigues
    rotated.x = vector.x * cosTheta + sinTheta * 
                (axis.y  * vector.z - axis.z   * vector.y) + 
                (1 - cosTheta) * (axis.x * dot(axis, vector));
    
    rotated.y = vector.y * cosTheta + sinTheta * 
                (axis.z  * vector.x - axis.x   * vector.z) + 
                (1 - cosTheta) * (axis.y * dot(axis, vector));
    
    rotated.z = vector.z * cosTheta + sinTheta *
                (axis.x  * vector.y - axis.y   * vector.x) + 
                (1 - cosTheta) * (axis.z * dot(axis, vector));

    return rotated;
}

static 
void debug_matrix(const Matrix4x4 *mat) 
{
    int i, j;

    printf("Matrix:\n");
    for(i = 0; i < 4; i++) 
    {
        for(j = 0; j < 4; j++) 
        {
            printf("%f ", mat->m44[i][j]);
        }
        printf("\n");
    }
}



#endif /* LOCALMATH_H */
