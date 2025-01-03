#ifndef _camera_h_
#define _camera_h_

#include"test_demo/localmath.h"

#include"3rdparty/glad/OpenGL/glad_gl.h"

typedef struct
{
    Matrix4x4 view;
    Matrix4x4 proj;

    Vector3 cameraPos;
    Vector3 cameraFront;
    Vector3 cameraUp;
    Vector3 cameraRight;

    float cameraSpeed;
    float yaw;
    float pitch;
    float roll;

    float sensitivity;

    float rollTarget;    // Valor para qual o roll deve retornar
    float rollSpeed;     // Velocidade de retorno em graus por segundo
    float rollMax;      // Limite máximo de roll em qualquer direção

    int Width, Height;

 /* Control Camera */
    int camForward;
    int camBackWard;
    int camElevate;
    int camLower;
    int camLeft;
    int camRight;
    int camSprint;
    int rollLeft;
    int rollRight;

} RCamera;

static 
int cameraSetup(RCamera *camera, int width, int height)
{
    //camera = (RCamera *)malloc(sizeof(RCamera));

    if(!camera) 
    {
        printf("\n RCamera nao valido cameraSetup() \n");
        return -1; // Verifica se a câmera é válida
    }

    camera->cameraPos.x = 0.0f;
    camera->cameraPos.y = 0.0f;
    camera->cameraPos.z = 3.0f;

    camera->cameraFront.x = 0.0f;
    camera->cameraFront.y = 0.0f;
    camera->cameraFront.z = -1.0f;

    camera->cameraUp.x = 0.0f;
    camera->cameraUp.y = 1.0f;
    camera->cameraUp.z = 0.0f;

    camera->cameraRight = normalize(cross(camera->cameraFront, camera->cameraUp));

    camera->cameraSpeed = 0.05f;

    camera->yaw = -90.0f;
    camera->pitch = 0.0f;
    camera->roll = 0.0f;

    camera->sensitivity = 0.1f;
    camera->Width = width;
    camera->Height = height;

        /* Controles */
    camera->camForward = 0;
    camera->camBackWard = 0;
    camera->camElevate = 0;
    camera->camLower = 0;
    camera->camLeft = 0;
    camera->camRight = 0;
    camera->camSprint = 0;
    camera->rollLeft = 0;
    camera->rollRight = 0;

    return 0;
}

static 
void cameraMatrix(RCamera     *camera, 
                  float        FOVdeg, 
                  float        nearPlane, 
                  float        farPlane)
{

    GLint uniformLocation;

    if(!camera) 
    {
        printf("\n RCamera nao valido cameraMatrix() \n");
        return; // Verifica se a câmera é válida
    }

    //camera = (RCamera *)malloc(sizeof(RCamera));

    camera->view = identity_matrix();
    camera->proj = identity_matrix();

    camera->view = look_at(camera->cameraPos.x, 
                   camera->cameraPos.y, 
                   camera->cameraPos.z,
                   camera->cameraPos.x + camera->cameraFront.x, 
                   camera->cameraPos.y + camera->cameraFront.y, 
                   camera->cameraPos.z + camera->cameraFront.z,
                   camera->cameraUp.x, 
                   camera->cameraUp.y, 
                   camera->cameraUp.z);

    camera->proj = perspective(FOVdeg * (PI / 180.0f), 
                               (float)camera->Width / camera->Height, 
                               nearPlane, 
                               farPlane);
    /*matrix4_projection_perspective_fovy_rh_EXP(&camera->proj, 
                                               FOVdeg * (PI / 180.0f), 
                                               (float)camera->Width / camera->Height, 
                                               nearPlane, 
                                               farPlane);*/

}

static
void sendCameraToRender(RCamera *camera, 
                        GLuint shaderID, 
                        const char *Uniform)
{

    if (!camera) 
    {
        printf("\n RCamera não válido em sendCameraToShader()\n");
        return;
    }


    //glUseProgram(shaderID);


    // Integração com Shader
    if (shaderID != 0 && Uniform != NULL)
    {

        // Envia para o shader como uniform
        //glUseProgram(shaderID);

        glUniformMatrix4fv(glGetUniformLocation(shaderID, Uniform), 1, GL_FALSE, &camera->view.m44[0][0]);

    }
    else
    {
        // Caso não tenha shader, configura diretamente a matriz de projeção e modelo-visão
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&camera->proj.m44[0][0]);

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(&camera->view.m44[0][0]);
    }

}

static 
void cameraInput(RCamera *camera, float mouseX, float mouseY, float deltaTime) 
{
    Vector3 direction;
    float xOffset, yOffset;
    float sprintMultiplier = 2.0f;

    if(!camera) 
    {
        printf("\n RCamera nao valido cameraInput() \n");
        return; // Verifica se a câmera é válida
    }

    xOffset = mouseX * camera->sensitivity;
    yOffset = -mouseY * camera->sensitivity; // Inverta o Y se necessário

    /* Ajusta a velocidade */
    camera->cameraSpeed = 2.5f * deltaTime;
    if(camera->camSprint) 
    {
        camera->cameraSpeed *= sprintMultiplier;
    }

    /* Controle de Roll */
    if(camera->rollLeft) 
    {
        camera->roll += camera->rollSpeed * deltaTime;
    }
    if(camera->rollRight)
    {
        camera->roll -= camera->rollSpeed * deltaTime;
    }

    /* Movimento da câmera */
    if(camera->camForward) 
    {
        camera->cameraPos = add(camera->cameraPos, scale(camera->cameraFront, camera->cameraSpeed));
    }
    if(camera->camBackWard) 
    {
        camera->cameraPos = subtract(camera->cameraPos, scale(camera->cameraFront, camera->cameraSpeed));
    }
    if(camera->camLeft) 
    {
        camera->cameraRight = normalize(cross(camera->cameraFront, camera->cameraUp));
        camera->cameraPos = subtract(camera->cameraPos, scale(camera->cameraRight, camera->cameraSpeed));
    }
    if(camera->camRight) 
    {
        camera->cameraRight = normalize(cross(camera->cameraFront, camera->cameraUp));
        camera->cameraPos = add(camera->cameraPos, scale(camera->cameraRight, camera->cameraSpeed));
    }
    if(camera->camElevate) 
    {
        camera->cameraPos = add(camera->cameraPos, scale(camera->cameraUp, camera->cameraSpeed));
    }
    if(camera->camLower) 
    {
        camera->cameraPos = subtract(camera->cameraPos, scale(camera->cameraUp, camera->cameraSpeed));
    }

    /* Mouse Look */
    camera->yaw += xOffset;
    camera->pitch += yOffset;

    /* Restringe o pitch */
    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;

    /* Atualiza a direção */
    direction.x = (float)cos(radians(camera->yaw)) * (float)cos(radians(camera->pitch));
    direction.y = (float)sin(radians(camera->pitch));
    direction.z = (float)sin(radians(camera->yaw)) * (float)cos(radians(camera->pitch));
    camera->cameraFront = normalize(direction);

    //camera->cameraRight = normalize(cross(camera->cameraFront, camera->cameraUp));
    //camera->cameraUp = rotate_vector3(camera->cameraUp, camera->cameraFront, radians(camera->roll));
}

static 
void cameraInputJStick(RCamera *camera, int analogPitch, int analogYaw, float deltaTime)
{
    Vector3 direction;
    float sprintMultiplier = 2.0f;
    const int DEADZONE = 25000; // O valor de centralização do joystick (baseado no que você informou)
    const float DEADZONE_THRESHOLD = 1000; // Valor mínimo que faz diferença de movimento
    float yawMovement = 0.0f, pitchMovement = 0.0f;

    if (!camera) 
    {
        printf("\n RCamera não válido cameraInput() \n");
        return;
    }

    // Verifica se o movimento está dentro da deadzone antes de aplicá-lo
    if (abs(analogYaw) < DEADZONE + DEADZONE_THRESHOLD) analogYaw = 0;
    if (abs(analogPitch) < DEADZONE + DEADZONE_THRESHOLD) analogPitch = 0;

    // Ajuste baseado nos controles analógicos (look-at)

    // Modificando o yaw e pitch com os eixos do controle (use os limites que você espera para esses valores)
    if(analogYaw != 0)
    {
        // Ajuste de rotação (yaw) baseado no eixo X do analógico direito
        yawMovement = (float)(analogYaw - DEADZONE) / (float)(32767 - DEADZONE); // Normaliza de -1 a 1
        camera->yaw += yawMovement * camera->sensitivity;
        //camera->yaw += analogYaw * camera->sensitivity;
    }

    if (analogPitch != 0)
    {
        // Ajuste de rotação (pitch) baseado no eixo Y do analógico direito
        //camera->pitch += analogPitch * camera->sensitivity;
        pitchMovement = (float)(analogPitch - DEADZONE) / (float)(32767 - DEADZONE); // Normaliza de -1 a 1
        camera->pitch += pitchMovement * camera->sensitivity;

        // Restringe o pitch
        if (camera->pitch > 89.0f) camera->pitch = 89.0f;
        if (camera->pitch < -89.0f) camera->pitch = -89.0f;
    }


    // Atualiza a direção após as mudanças no yaw/pitch
    direction.x = (float)cos(radians(camera->yaw)) * (float)cos(radians(camera->pitch));
    direction.y = (float)sin(radians(camera->pitch));
    direction.z = (float)sin(radians(camera->yaw)) * (float)cos(radians(camera->pitch));
    camera->cameraFront = normalize(direction);
}

#endif /* _camera_h_ */
