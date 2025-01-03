
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501 // Windows XP or superior
#include<windows.h>


#include<stdio.h>

#include"core/input_manager.h"

void convertWindowCoordsToOpenGL(int winX, 
                                 int winY, 
                                 int winWidth, 
                                 int winHeight, 
                                 float *glX, 
                                 float *glY) 
{
    // Convertendo coordenadas da janela para o intervalo de [0, 1]
    float normX = (float)winX / (float)winWidth;
    float normY = (float)winY / (float)winHeight;

    // Convertendo coordenadas normalizadas para o intervalo de [-1, 1]
    *glX = normX * 2.0f - 1.0f;
    *glY = 1.0f - normY * 2.0f;
}

void updateMouseState(HWND hwnd, MouseState *mouseState) 
{
    // Obter as coordenadas atuais do mouse
    POINT currentPos;
    RECT windowRect;
    int newX , 
        newY;
    int winWidth, 
        winHeight;
    GetCursorPos(&currentPos);
    ScreenToClient(hwnd, &currentPos);

    GetClientRect(hwnd, &windowRect);
    winWidth  = windowRect.right - windowRect.left;
    winHeight = windowRect.bottom - windowRect.top;

    // Atualizar as coordenadas atuais do mouse
        newX = currentPos.x;
        newY = currentPos.y;

    // Converter coordenadas da janela para coordenadas OpenGL
    convertWindowCoordsToOpenGL(currentPos.x, 
                                currentPos.y, 
                                winWidth, 
                                winHeight, 
                                &mouseState->currentX, 
                                &mouseState->currentY);

    // Calcular o movimento relativo do mouse desde a última atualização
    mouseState->xrel = newX - mouseState->prevX;
    mouseState->yrel = newY - mouseState->prevY;

    // Atualizar as coordenadas anteriores do mouse para a próxima atualização
    mouseState->prevX = newX;
    mouseState->prevY = newY;

    // Se o mouse se moveu, imprimir as novas coordenadas e o movimento relativo
    if (mouseState->xrel != 0 || mouseState->yrel != 0) 
    {
        //printf("Mouse moved to: (%d, %d)\n", newX, newY);
        //printf("Relative movement: (%d, %d)\n", mouseState->xrel, mouseState->yrel);
    }
}

void confineMouseToWindow(HWND hwnd) 
{
    RECT rect;
    GetClientRect(hwnd, &rect);
    ClientToScreen(hwnd, (POINT*)&rect.left);
    ClientToScreen(hwnd, (POINT*)&rect.right);
    ClipCursor(&rect); // Restringe o cursor à área da janela
}

void resetMouseToCenter(HWND hwnd) 
{
    RECT rect;
    POINT center;
    GetClientRect(hwnd, &rect);
    // Calcular o centro da janela
    center.x = (rect.right - rect.left) / 2;
    center.y = (rect.bottom - rect.top) / 2;
    ClientToScreen(hwnd, &center);
    SetCursorPos(center.x, center.y); // Move o cursor para o centro
}

void initMouseEvent(struct MouseEvent *event) 
{
    if(!event) return;
        event->mouseX = 0;
        event->mouseY = 0;
        event->xrel = 0;
        event->yrel = 0;
        event->lastMouseX = 0;
        event->lastMouseY = 0;
        event->mouseMotion = 0;
        event->mouseLeft = 0;
        event->mouseRight = 0;
        event->mouseMiddle = 0;
        event->buttonState = 0;
        event->clicks = 0;
        event->windowFocusID = 0;
        event->mouseInstanceID = 0;
}

void updateMouseEvent(HWND hwnd, struct MouseEvent *event) 
{
    POINT mousePos;
    RECT windowRect;
    
    if (!event) return;


    GetCursorPos(&mousePos);
    ScreenToClient(hwnd, &mousePos);

    GetClientRect(hwnd, &windowRect);

    if (PtInRect(&windowRect, mousePos)) 
    {
        event->mouseX = mousePos.x;
        event->mouseY = mousePos.y;
        event->xrel = mousePos.x - event->lastMouseX;
        event->yrel = mousePos.y - event->lastMouseY;
        event->mouseMotion = (event->xrel != 0 || event->yrel != 0);
        event->lastMouseX = mousePos.x;
        event->lastMouseY = mousePos.y;
        
        // Estado dos botões do mouse
        event->mouseLeft   = (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
        event->mouseRight  = (GetKeyState(VK_RBUTTON) & 0x8000) != 0;
        event->mouseMiddle = (GetKeyState(VK_MBUTTON) & 0x8000) != 0;

        // Atualizar botãoState usando uma máscara de bits
        event->buttonState = 0;
        if (event->mouseLeft) event->buttonState   |= 0x01;
        if (event->mouseRight) event->buttonState  |= 0x02;
        if (event->mouseMiddle) event->buttonState |= 0x04;
    }
}

int mouseInputPos(HWND hwnd, BOOL inputEnabled, int *x, int *y)
{
    RECT windowRect;
    POINT mousePos;

    if(inputEnabled)
    {
        /* Get the x and y coordinates of the mouse relative to the screen */
        GetCursorPos(&mousePos);

        /* Convert coordinates to window-relative coordinates */
        ScreenToClient(hwnd, &mousePos);

        GetClientRect(hwnd, &windowRect);

        if (PtInRect(&windowRect, mousePos))
        {
            /* The mouse is within the window limits */
            static int lastMouseX = 0;
            static int lastMouseY = 0;

            if (mousePos.x != lastMouseX || mousePos.y != lastMouseY)
            {
                lastMouseX = mousePos.x;
                lastMouseY = mousePos.y;
                *x = lastMouseX; // Atualiza o valor apontado pelo ponteiro x
                *y = lastMouseY; // Atualiza o valor apontado pelo ponteiro y
                return 1; // Retorna 1 indicando que o mouse está em movimento
            }
        }
    }
    return 0; // Retorna 0 indicando que o mouse está parado
}

void setInputEnabled(int enabled)
{
    if (enabled == true) 
    {
        /* Enable input */
    } 
    else if (enabled == false) 
    {       /* Disable input*/ 
	
	} 
     else 
    {
        /* Treat if it is an invalid value (optional) */
    }

    inputEnabled = enabled;
}



/*
DWORD keyInput(int keyCode, InputMode mode) 
{
    int keyState[256] = { FALSE };  // Array para controlar o estado das teclas
    int prevKeyState[256] = { FALSE }; // Array para armazenar o estado anterior das teclas
    DWORD keyPressTimes[256] = { 0 }; // Array para registrar os tempos de pressão das teclas
    DWORD releaseTime = 0;
    DWORD pressTime = 0;
    DWORD pressDuration = 0;

    // Obter o estado atual da tecla
    int currentKeyState = GetAsyncKeyState(keyCode) & PRESS_STATE;

    // Verificar se a tecla está pressionada no momento atual
    if(currentKeyState) 
    {
        // Verificar se a tecla estava solta no frame anterior
        if(!prevKeyState[keyCode]) 
        {
            // Marcar a tecla como pressionada no frame atual
            keyState[keyCode] = true;
            keyPressTimes[keyCode] = GetTickCount(); // Registra o tempo de pressão da tecla
            prevKeyState[keyCode] = true;

            if (mode == SINGLE) 
            {
               return true;
            }
        }
    } 
    else 
    {
        if (keyState[keyCode]) 
        {
            prevKeyState[keyCode] = FALSE; // Marcar a tecla como solta no frame atual

            if (mode == SINGLE) 
            {
                keyState[keyCode] = FALSE;
            }

            releaseTime = GetTickCount();
            pressTime = keyPressTimes[keyCode];
            pressDuration = releaseTime - pressTime;

           if (mode == KEEP_TIME) 
            {
                return pressDuration;
                keyState[keyCode] = false;
            }            

        }
    }

    // Retornar 0 se o modo KEEP ou KEEP_TIME for usado e a tecla estiver pressionada
    return (mode == KEEP) && currentKeyState ? 1 : 0;

}
*/


