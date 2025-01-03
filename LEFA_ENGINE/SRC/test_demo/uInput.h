/*Open LEFA : User Input
2023*/

#ifndef UINPUT_H
#define UINPUT_H

#ifdef APIENTRY
    #undef APIENTRY
#endif

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501 // Windows XP or superior
#include<windows.h>

#include<stdio.h>
#include<stdbool.h>


#define true 1
#define TRUE 1
#define false 0
#define FALSE 0
typedef int bool_;

extern HWND hwnd;
static bool_ inputEnabled = true;

extern int windowWidth;
extern int windowHeight;

#define BUTTON_RELEASED 0
#define BUTTON_PRESSED  1



typedef struct 
{
    float prevX;
    float prevY;
    float currentX;
    float currentY;
    float xrel;
    float yrel;
}MouseState;

static void convertWindowCoordsToOpenGL(int winX, 
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


static void updateMouseState(HWND hwnd, MouseState *mouseState) 
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



void registerRawInput(HWND hwnd) 
{
    RAWINPUTDEVICE rid;

    rid.usUsagePage = 0x01;  // Genérico
    rid.usUsage = 0x02;      // Mouse
    rid.dwFlags = RIDEV_INPUTSINK; // Capturar mesmo quando fora de foco
    rid.hwndTarget = hwnd;

    if (!RegisterRawInputDevices(&rid, 1, sizeof(rid))) 
    {
        MessageBox(hwnd, "Falha ao registrar Raw Input.", "Erro", MB_OK | MB_ICONERROR);
    }
}

struct MouseEvent 
{
    int mouseX;                     // Posição atual do mouse em X
    int mouseY;                     // Posição atual do mouse em Y
    int xrel;                       // Movimento relativo do mouse em X
    int yrel;                       // Movimento relativo do mouse em Y
    int lastMouseX;                 // Última posição X do mouse
    int lastMouseY;                 // Última posição Y do mouse
    int mouseMotion;                // Flag para indicar movimento do mouse
    int mouseLeft;                  // Estado do botão esquerdo do mouse
    int mouseRight;                 // Estado do botão direito do mouse
    int mouseMiddle;                // Estado do botão do meio do mouse
    int buttonState;                // Estado dos botões do mouse (bitmask)
    unsigned char clicks;           // Número de cliques (simples ou duplo)
    unsigned int windowFocusID;     // ID da janela com foco
    unsigned int mouseInstanceID;   // ID da instância do mouse
};

static void initMouseEvent(struct MouseEvent *event) 
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

static void updateMouseEvent(HWND hwnd, struct MouseEvent *event) 
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


static int mouseInputPos(HWND hwnd, BOOL inputEnabled, int *x, int *y)
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


static void setInputEnabled(int enabled)
{
    if (enabled == true) 
    {
        /* Enable input */
    } 
    else if (enabled == false) 
    {       /* Disable input*/ } 
     else 
    {
        /* Treat if it is an invalid value (optional) */
    }

    inputEnabled = enabled;
}



enum keyMaps 
{
/* Mouse */
    KEY_MS1  = 0x01,   /* Left button   */
    KEY_MS2  = 0x02,   /* Right button  */
    KEY_MS3  = 0x04,   /* Middle button */
  /*KEY_MS4  = 0000,   /* otherbuttons  */
  /*KEY_MS5  = 0000,   /* otherbuttons  */

/* Keyboard */
    /* Special Keys */
    KEY_ESC  = 0x1B,   /* Escape */
    KEY_F1   = 0x70,   /* F1 */
    KEY_F2   = 0x71,   /* F2 */
    KEY_F3   = 0x72,   /* F3 */
    KEY_F4   = 0x73,   /* F4 */
    KEY_F5   = 0x74,   /* F5 */
    KEY_F6   = 0x75,   /* F6 */
    KEY_F7   = 0x76,   /* F7 */
    KEY_F8   = 0x77,   /* F8 */
    KEY_F9   = 0x78,   /* F9 */
    KEY_F10  = 0x79,   /* F10 */
    KEY_F11  = 0x7A,   /* F11 */
    KEY_F12  = 0x7B,   /* F12 */
    KEY_BSPC = 0x08,   /* Backspace */
    KEY_TAB  = 0x09,   /* TAB Key */
    KEY_CPSL = 0x14,   /* Caps lock */
    KEY_SHIFT = 0x10,  /* Shift Key */
    KEY_CTRL = 0x11,   /* Control Key */
    KEY_ALT  = 0x12,   /* Alt Keys */
    KEY_SPC  = 0x20,   /* Space Key */
    KEY_ENTR = 0x0D,   /* Enter Key */
    KEY_LFAR = 0x25,   /* <  left arrow */
    KEY_UPAR = 0x26,   /* ^  Up arrow */
    KEY_RIAR = 0x27,   /* >  Right Arrow  */
    KEY_DWAR = 0x28,   /* ˅  Down Arrow */
    /* double Keys/key combination/secondary function */
    KEY_HYPH = 0xBD,   /* OEM_MINUS (_ -) */
    /*
    */
    KEY_QUOT = 0xDE,   /* Quote ' " */
    /* KEY_DQ   0x22   /* Double Quote " = KEY_DQ */
    /*
    */
    KEY_0    = 0x30,   /* 0 */
    KEY_1    = 0x31,   /* 1 */
    KEY_2    = 0x32,   /* 2 */
    KEY_3    = 0x33,   /* 3 */
    KEY_4    = 0x34,   /* 4 */
    KEY_5    = 0x35,   /* 5 */
    KEY_6    = 0x36,   /* 6 */
    KEY_7    = 0x37,   /* 7 */
    KEY_8    = 0x38,   /* 8 */
    KEY_9    = 0x39,   /* 9 */
    /*
    */
    KEY_A    = 0x41,
    KEY_B    = 0x42,
    KEY_C    = 0x43,
    KEY_D    = 0x44,
    KEY_E    = 0x45,
    KEY_F    = 0x46,
    KEY_G    = 0x47,
    KEY_H    = 0x48,
    KEY_I    = 0x49,
    KEY_J    = 0x4A,
    KEY_K    = 0x4B,
    KEY_L    = 0x4C,
    KEY_M    = 0x4D,
    KEY_N    = 0x4E,
    KEY_O    = 0x4F,
    KEY_P    = 0x50,
    KEY_Q    = 0x51,
    KEY_R    = 0x52,
    KEY_S    = 0x53,
    KEY_T    = 0x54,
    KEY_U    = 0x55,
    KEY_V    = 0x56,
    KEY_W    = 0x57,
    KEY_X    = 0x58,
    KEY_Y    = 0x59,
    KEY_Z    = 0x5A,

};



/*
static void mouseInput()
{
    RECT windowRect;
    if(inputEnabled)
    {
        /* Get the x and y coordinates of the mouse relative to the screen *
        POINT mousePos;
        GetCursorPos(&mousePos);

        /* Convert coordinates to window-relative coordinates *
        ScreenToClient(hwnd, &mousePos);

        GetClientRect(hwnd, &windowRect);

        if (PtInRect(&windowRect, mousePos))
      {
        /* The mouse is within the window limits *
        static int lastMouseX = 0;
        static int lastMouseY = 0;

        if (mousePos.x != lastMouseX || mousePos.y != lastMouseY)
        {
            lastMouseX = mousePos.x;
            lastMouseY = mousePos.y;
            /* printf("Mouse Pos: x = %d, y = %d\n", lastMouseX, lastMouseY); 
            *
        }

       }
    }
}
*/

#define PRESS_STATE 0x8000

typedef enum 
{
    SINGLE,
    KEEP,
    KEEP_TIME
} InputMode;



static DWORD keyInput(int keyCode, InputMode mode) 
{
    static bool_ keyState[256] = { FALSE };  // Array para controlar o estado das teclas
    static bool_ prevKeyState[256] = { FALSE }; // Array para armazenar o estado anterior das teclas
    static DWORD keyPressTimes[256] = { 0 }; // Array para registrar os tempos de pressão das teclas
    DWORD releaseTime = 0;
    DWORD pressTime = 0;
    DWORD pressDuration = 0;

    // Obter o estado atual da tecla
    bool_ currentKeyState = GetAsyncKeyState(keyCode) & PRESS_STATE;

    // Verificar se a tecla está pressionada no momento atual
    if (currentKeyState) 
    {
        // Verificar se a tecla estava solta no frame anterior
        if (!prevKeyState[keyCode]) 
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




/* 
static bool_ keyInput(int key, DWORD* pressTimeLimits, int numActions) 
{
    static bool keyStates[256] = { false }; // Array para controlar o estado das teclas
    static DWORD keyPressTimes[256] = { 0 }; // Array para registrar os tempos de pressão das teclas
    DWORD releaseTime = 0;
    DWORD pressTime = 0;
    DWORD pressDuration = 0;
    int i = 0;

    if (!inputEnabled) return false;

    if (GetAsyncKeyState(key) & PRESS_STATE) 
    {
        if (!keyStates[key]) 
        { // Verifica se a tecla foi pressionada recentemente
            keyStates[key] = true;
            keyPressTimes[key] = GetTickCount(); // Registra o tempo de pressão da tecla
            return true;
        }
    } 
    else 
    {
        if (keyStates[key]) 
        {
            keyStates[key] = false;
            releaseTime = GetTickCount(); // Tempo atual de liberação da tecla
            pressTime = keyPressTimes[key]; // Tempo de pressão registrado da tecla
            pressDuration = releaseTime - pressTime; // Duração da pressão da tecla

            if (pressDuration < 500) 
            {
                // Pisca em branco se o botão for pressionado rapidamente
                printf("Tecla %d pressionada por menos de %d ms\n", key, pressTimeLimits[i]);
            } 
            else 
            {
                for (i = 0; i < numActions; ++i) 
                {
                    if (pressDuration < pressTimeLimits[i]) 
                    {
                        // Realize a ação correspondente ao tempo de pressionamento
                        printf("Tecla %d pressionada por menos de %d ms\n", key, pressTimeLimits[i]);
                        break;
                    } 
                    else if (i == numActions - 1) 
                    {
                        // Realize uma ação padrão se nenhum limite for atingido
                        printf("Tecla %d pressionada por mais de %d ms\n", key, pressTimeLimits[i]);
                    }
                }
            }

            // Informa o tempo exato que a tecla foi pressionada
            printf("Tempo de pressionamento da tecla %d: %d ms\n", key, pressDuration);
        }
    }

    return false;
}
 */


/*
static int keyInput(int key)
{
    static bool_ keyStates[256] = { false }; // Array para controlar o estado das teclas

    if(!inputEnabled) return false;

        if (GetAsyncKeyState(key) & PRESS_STATE) 
        {
        if (!keyStates[key]) { // Verifica se a tecla foi pressionada pela primeira vez
            keyStates[key] = true;
            return true;
        }
    } else 
    {
        keyStates[key] = false; // Marca a tecla como não pressionada
    }

    return false;
    //return GetAsyncKeyState(key) & PRESS_STATE;
}
*/



#endif /* UINPUT_H */

