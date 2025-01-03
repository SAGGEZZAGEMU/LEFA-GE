#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#ifdef APIENTRY
	#undef APIENTRY
#endif

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0501 // Windows XP or superior
#include<windows.h>

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdarg.h>

//#include"core/window_manager.h"

#define BUTTON_RELEASED 0
#define BUTTON_PRESSED  1

static bool inputEnabled = true;

typedef struct 
{
    float prevX;
    float prevY;
    float currentX;
    float currentY;
    float xrel;
    float yrel;
}MouseState;

struct KeyState 
{
    bool isActive;
    bool isReleased;
    int  mouseX;
    int  mouseY;

};

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

/*
enum keyMaps 
{
/* Mouse *
    KEY_MS1  = 0x01,   /* Left button   *
    KEY_MS2  = 0x02,   /* Right button  *
    KEY_MS3  = 0x04,   /* Middle button *
  /*KEY_MS4  = 0000,   /* otherbuttons  *
  /*KEY_MS5  = 0000,   /* otherbuttons  *

/* Keyboard */
    /* Special Keys *
    KEY_ESC  = 0x1B,   /* Escape *
    KEY_F1   = 0x70,   /* F1 *
    KEY_F2   = 0x71,   /* F2 *
    KEY_F3   = 0x72,   /* F3 *
    KEY_F4   = 0x73,   /* F4 *
    KEY_F5   = 0x74,   /* F5 *
    KEY_F6   = 0x75,   /* F6 *
    KEY_F7   = 0x76,   /* F7 *
    KEY_F8   = 0x77,   /* F8 *
    KEY_F9   = 0x78,   /* F9 *
    KEY_F10  = 0x79,   /* F10 *
    KEY_F11  = 0x7A,   /* F11 *
    KEY_F12  = 0x7B,   /* F12 *
    KEY_BSPC = 0x08,   /* Backspace *
    KEY_TAB  = 0x09,   /* TAB Key *
    KEY_CPSL = 0x14,   /* Caps lock *
    KEY_SHIFT = 0x10,  /* Shift Key *
    KEY_CTRL = 0x11,   /* Control Key *
    KEY_ALT  = 0x12,   /* Alt Keys *
    KEY_SPC  = 0x20,   /* Space Key *
    KEY_ENTR = 0x0D,   /* Enter Key *
    KEY_LFAR = 0x25,   /* <  left arrow *
    KEY_UPAR = 0x26,   /* ^  Up arrow *
    KEY_RIAR = 0x27,   /* >  Right Arrow  *
    KEY_DWAR = 0x28,   /* ˅  Down Arrow *
    /* double Keys/key combination/secondary function *
    KEY_HYPH = 0xBD,   /* OEM_MINUS (_ -) *
    /*
    *
    KEY_QUOT = 0xDE,   /* Quote ' " *
    /* KEY_DQ   0x22   /* Double Quote " = KEY_DQ *
    /*
    *
    KEY_0    = 0x30,   /* 0 *
    KEY_1    = 0x31,   /* 1 *
    KEY_2    = 0x32,   /* 2 *
    KEY_3    = 0x33,   /* 3 *
    KEY_4    = 0x34,   /* 4 *
    KEY_5    = 0x35,   /* 5 *
    KEY_6    = 0x36,   /* 6 *
    KEY_7    = 0x37,   /* 7 *
    KEY_8    = 0x38,   /* 8 *
    KEY_9    = 0x39,   /* 9 *
    /*
    *
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
*/

enum keyMaps {
    /* Mouse */
    KEY_MS1  = 0x00,   /* Left button   */
    KEY_MS2  = 0x01,   /* Right button  */
    KEY_MS3  = 0x02,   /* Middle button */
    KEY_MS4  = 0x03,   /* Side button 1 (Mouse 4) */
    KEY_MS5  = 0x04,   /* Side button 2 (Mouse 5) */

    /* Special Keys */
    KEY_ESC  = 0x01,   /* Escape */
    KEY_F1   = 0x3B,   /* F1 */
    KEY_F2   = 0x3C,   /* F2 */
    KEY_F3   = 0x3D,   /* F3 */
    KEY_F4   = 0x3E,   /* F4 */
    KEY_F5   = 0x3F,   /* F5 */
    KEY_F6   = 0x40,   /* F6 */
    KEY_F7   = 0x41,   /* F7 */
    KEY_F8   = 0x42,   /* F8 */
    KEY_F9   = 0x43,   /* F9 */
    KEY_F10  = 0x44,   /* F10 */
    KEY_F11  = 0x57,   /* F11 */
    KEY_F12  = 0x58,   /* F12 */
    KEY_BSPC = 0x0E,   /* Backspace */
    KEY_TAB  = 0x0F,   /* TAB Key */
    KEY_CPSL = 0x3A,   /* Caps Lock */
    KEY_LSHIFT = 0x2A,  /* Left Shift */
    KEY_RSHIFT = 0x36,  /* Right Shift */
    KEY_LCTRL = 0x1D,  /* Left Control */
    KEY_RCTRL = 0xE01D, /* Right Control */
    KEY_LALT  = 0x38,   /* Left Alt */
    KEY_RALT  = 0xE038, /* Right Alt */
    KEY_SPC  = 0x39,   /* Space */
    KEY_ENTR = 0x1C,   /* Enter */
    KEY_LFAR = 0xCB,   /* Left Arrow */
    KEY_UPAR = 0xC8,   /* Up Arrow */
    KEY_RIAR = 0xCD,   /* Right Arrow */
    KEY_DWAR = 0xD0,   /* Down Arrow */

    /* Numbers */
    KEY_0    = 0x0B,
    KEY_1    = 0x02,
    KEY_2    = 0x03,
    KEY_3    = 0x04,
    KEY_4    = 0x05,
    KEY_5    = 0x06,
    KEY_6    = 0x07,
    KEY_7    = 0x08,
    KEY_8    = 0x09,
    KEY_9    = 0x0A,

    /* Letters */
    KEY_A    = 0x1E,
    KEY_B    = 0x30,
    KEY_C    = 0x2E,
    KEY_D    = 0x20,
    KEY_E    = 0x12,
    KEY_F    = 0x21,
    KEY_G    = 0x22,
    KEY_H    = 0x23,
    KEY_I    = 0x17,
    KEY_J    = 0x24,
    KEY_K    = 0x25,
    KEY_L    = 0x26,
    KEY_M    = 0x32,
    KEY_N    = 0x31,
    KEY_O    = 0x18,
    KEY_P    = 0x19,
    KEY_Q    = 0x10,
    KEY_R    = 0x13,
    KEY_S    = 0x1F,
    KEY_T    = 0x14,
    KEY_U    = 0x16,
    KEY_V    = 0x2F,
    KEY_W    = 0x11,
    KEY_X    = 0x2D,
    KEY_Y    = 0x15,
    KEY_Z    = 0x2C
};


#define PRESS_STATE 0x8000

typedef enum 
{
    SINGLE,
    KEEP,
    KEEP_TIME
} InputMode;

static 
float mouseDeltaX = 0.0f, 
      mouseDeltaY = 0.0f;



void initMouseEvent(struct MouseEvent *event);

void updateMouseEvent(HWND hwnd, struct MouseEvent *event);

int mouseInputPos(HWND hwnd, BOOL inputEnabled, int *x, int *y);

void setInputEnabled(int enabled);

// DWORD keyInput(int keyCode, InputMode mode); /* windows winuser keys */ 

void convertWindowCoordsToOpenGL(int winX, 
                                 int winY, 
                                 int winWidth, 
                                 int winHeight, 
                                 float *glX, 
                                 float *glY);


void updateMouseState(HWND hwnd, MouseState *mouseState);

void confineMouseToWindow(HWND hwnd);

void resetMouseToCenter(HWND hwnd);

static 
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

static 
void processRawInput(LPARAM lParam) /* FIXME! : lag input */
{
    UINT dwSize = 0;
    RAWINPUT *raw = NULL;

    // Obtenha o tamanho necessário para armazenar os dados do RawInput
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
    if(dwSize == 0) return;

    raw = (RAWINPUT *)malloc(dwSize);
    if(!raw) 
    {
        fprintf(stderr, "Falha ao alocar memória para RAWINPUT.\n");
        return;
    }

    // Obtenha os dados do RawInput
    if(GetRawInputData((HRAWINPUT)lParam, RID_INPUT, raw, &dwSize, sizeof(RAWINPUTHEADER)) == dwSize) 
    {
        if(raw->header.dwType == RIM_TYPEMOUSE) 
        {
            // Obtenha os movimentos relativos do mouse
            mouseDeltaX = (float)raw->data.mouse.lLastX;
            mouseDeltaY = (float)raw->data.mouse.lLastY;

            // Opcional: Adicione depuração para ver os valores
            printf("Movimento relativo: DeltaX = %.2f, DeltaY = %.2f\n", mouseDeltaX, mouseDeltaY);
        }
    } 
    else 
    {
        fprintf(stderr, "Falha ao processar WM_INPUT.\n");
    }

    free(raw); // Libere a memória
}

#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>

#include"test_demo/kenklock.h"

/*
LPDIRECTINPUT8 dInput = NULL; //dInput->lpVtbl->
LPDIRECTINPUTDEVICE8 dInputMouse = NULL;
DIMOUSESTATE mouseState;



HRESULT InitDirectInput(HINSTANCE hInstance, HWND hwnd) 
{
    HRESULT hr;

    // Inicializar DirectInput
    hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, (REFIID)&IID_IDirectInput8, (void**)&dInput, NULL);
    if (FAILED(hr)) 
    {
        MessageBox(hwnd, "Erro ao inicializar DirectInput.", "Erro", MB_OK | MB_ICONERROR);
        return hr;
    }

    // Criar dispositivo de mouse
    hr = dInput->lpVtbl->CreateDevice(dInput,
                                      &GUID_SysMouse, 
                                      &dInputMouse, 
                                      NULL);
    if (FAILED(hr)) 
    {
        MessageBox(hwnd, "Erro ao criar dispositivo de mouse.", "Erro", MB_OK | MB_ICONERROR);
        return hr;
    }

    // Configurar o formato dos dados do dispositivo
    hr = dInputMouse->lpVtbl->SetDataFormat(dInputMouse ,&c_dfDIMouse);
    if (FAILED(hr)) 
    {
        MessageBox(hwnd, "Erro ao configurar formato de dados do mouse.", "Erro", MB_OK | MB_ICONERROR);
        return hr;
    }

    // Configurar o nível cooperativo
    hr = dInputMouse->lpVtbl->SetCooperativeLevel(dInputMouse, hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr)) 
    {
        MessageBox(hwnd, "Erro ao configurar nível cooperativo.", "Erro", MB_OK | MB_ICONERROR);
        return hr;
    }

    // Adquirir o dispositivo
    hr = dInputMouse->lpVtbl->Acquire(dInputMouse);
    if (FAILED(hr)) 
    {
        MessageBox(hwnd, "Erro ao adquirir o dispositivo.", "Erro", MB_OK | MB_ICONERROR);
        return hr;
    }

    return S_OK;
}


void CleanupDirectInput() 
{
    if(dInputMouse) 
    {
        dInputMouse->lpVtbl->Unacquire(dInputMouse);
        dInputMouse->lpVtbl->Release(dInputMouse);
        dInputMouse = NULL;
    }
    if(dInput) 
    {
        dInput->lpVtbl->Release(dInput);
        dInput = NULL;
    }
}
*/

//DirectInput VARIABLES & CODE-------------------------------------------------------
static char keystatus[256];
static long shkeystatus = 0;
#define KEYBUFSIZ 256
static long keybuf[KEYBUFSIZ], keybufr = 0, keybufw = 0, keybufw2 = 0;

static char ext_keystatus[256]; // +TD
static char ext_mbstatus[8] = {0}; // +TD extended mouse button status
static long ext_mwheel = 0;
//#ifdef NOINPUT
//long mouse_acquire = 0;
//#else
static long mouse_acquire = 1, kbd_acquire = 1;
static void (*setmousein)(long, long) = NULL;
static long mouse_out_x, 
            mouse_out_y;
static HANDLE dinputevent[2] = {0,0};


static LPDIRECTINPUT8A gpdi = 0;

static DIMOUSESTATE2 mouseState; // Suporte para até 8 botões

static 
long initdirectinput(HWND hwnd)
{
    HRESULT hr;
    char buf[256];

    if((hr = DirectInput8Create(GetModuleHandle(NULL), 
                                 DIRECTINPUT_VERSION, 
                                 &IID_IDirectInput8A,
                                 (LPVOID *)&gpdi, 
                                 NULL)) >= 0) 
    {
        return(1);
    }
    wsprintf(buf,"initdirectinput failed: %08lx\n",hr);
    MessageBox(hwnd, buf,"ERROR",MB_OK);
    return(0);
}

static 
void uninitdirectinput()
{
    if(gpdi) 
    { 
        gpdi->lpVtbl->Release(gpdi); 
        gpdi = 0; 
    }
}


/* mouse */
   static  //long dinputmouseflags = DISCL_EXCLUSIVE|DISCL_FOREGROUND;
    long dinputmouseflags = DISCL_NONEXCLUSIVE | DISCL_FOREGROUND;

    static LPDIRECTINPUTDEVICE8 gpMouse = 0;
    #define MOUSBUFFERSIZE 64
    static DIDEVICEOBJECTDATA MousBuffer[MOUSBUFFERSIZE];
    static long gbstatus = 0, gkillbstatus = 0;

    //Mouse smoothing variables:
    static long mousmoth = 1;
    static double dmoutsc;
    static float mousper;
    static float mousince, mougoalx, mougoaly, mougoalz, moutscale;
    static long moult[4], moultavg, moultavgcnt;

    static
    void uninitmouse()
    {
        if(gpMouse)
        {
            if (dinputevent[0])
            {
                gpMouse->lpVtbl->SetEventNotification(gpMouse, dinputevent[0]);
                CloseHandle(dinputevent[0]); dinputevent[0] = 0;
            }
            gpMouse->lpVtbl->Unacquire(gpMouse); 
            gpMouse->lpVtbl->Release(gpMouse); 
            gpMouse = 0;
        }
    }

    static 
    long initmouse(HWND hwnd)
    {
        HRESULT hr;
        DIPROPDWORD dipdw;
        char buf[256];

        if((hr = gpdi->lpVtbl->CreateDevice(gpdi, &GUID_SysMouse, &gpMouse,0)) < 0)
        {
         goto initmouse_bad;
        }

        if((hr = gpMouse->lpVtbl->SetDataFormat(gpMouse, &c_dfDIMouse)) < 0)
        {
         goto initmouse_bad;
        }
        
        if ((hr = gpMouse->lpVtbl->SetDataFormat(gpMouse, &c_dfDIMouse2)) < 0) 
        {
            goto initmouse_bad;
        }

        if((hr = gpMouse->lpVtbl->SetCooperativeLevel(gpMouse, hwnd, dinputmouseflags)) < 0) 
        {
            goto initmouse_bad;
        }

        dinputevent[0] = CreateEvent(0,0,0,0); 
        if(!dinputevent[0]) 
        {
            goto initmouse_bad;
        }
        
        if((hr = gpMouse->lpVtbl->SetEventNotification(gpMouse, dinputevent[0])) < 0) 
        {
            goto initmouse_bad;
        }

        dipdw.diph.dwSize = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj = 0;
        dipdw.diph.dwHow = DIPH_DEVICE;
        dipdw.dwData = MOUSBUFFERSIZE;
        if((hr = gpMouse->lpVtbl->SetProperty(gpMouse, DIPROP_BUFFERSIZE, &dipdw.diph)) < 0) 
        {
            goto initmouse_bad;
        }

        if(mouse_acquire) 
        { 
            gpMouse->lpVtbl->Acquire(gpMouse); 
            gbstatus = 0; 
        }
        mousper = 1.0; 
        mousince = mougoalx = mougoaly = mougoalz = 0.0;
        moult[0] = -1; 
        moultavg = moultavgcnt = 0;
        readklock(&dmoutsc);
        return(1);

    initmouse_bad:;
        uninitmouse();
        wsprintf(buf,"initdirectinput(mouse) failed: %08lx\n",hr);
        MessageBox(hwnd, buf,"ERROR",MB_OK);
        return(0);
    }

    static
    void readmouse(float *fmousx, float *fmousy, float *fmousz, long *bstatus)
    {
        double odmoutsc;
        float f, fmousynctics;
        long i, got, ltmin, ltmax, nlt0, nlt1, nlt2;
        long mousx, mousy, mousz;
        HRESULT hr;
        unsigned long dwItems = MOUSBUFFERSIZE;
        DIDEVICEOBJECTDATA *lpdidod;


        if ((!mouse_acquire) || (!gpMouse)) 
        { 
            *fmousx = 0; 
            *fmousy = 0; 
            *fmousz = 0; 
            *bstatus = 0; 
            return; 
        }

        hr = gpMouse->lpVtbl->GetDeviceState(gpMouse, sizeof(DIMOUSESTATE2), &mouseState);
        if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) 
        {
            gpMouse->lpVtbl->Acquire(gpMouse); // Reaquira o dispositivo
            *fmousx = 0;
            *fmousy = 0;
            *fmousz = 0;
            *bstatus = 0;
            return;
        }


        dwItems = MOUSBUFFERSIZE;
        hr = gpMouse->lpVtbl->GetDeviceData(gpMouse, sizeof(DIDEVICEOBJECTDATA),MousBuffer,&dwItems,0);
        if(hr == DI_BUFFEROVERFLOW) moult[0] = -1;
        if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
        {
            gpMouse->lpVtbl->Acquire(gpMouse); gbstatus = 0;
            hr = gpMouse->lpVtbl->GetDeviceData(gpMouse, sizeof(DIDEVICEOBJECTDATA),MousBuffer,&dwItems,0);
            if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED)) 
            { 
                *fmousx = 0; 
                *fmousy = 0; 
                *fmousz = 0; 
                *bstatus = 0; 
                return; 
            }
        }

        //Estimate mouse period (mousper) in units of CPU cycles:
        mousx = mousy = mousz = 0; 
        got = 0;
        i = 0; lpdidod = &MousBuffer[i];
        
        while(i < (long)dwItems)
        {
            moult[3] = moult[2]; moult[2] = moult[1]; moult[1] = moult[0];
            moult[0] = lpdidod->dwTimeStamp;
            do
            {
                switch(lpdidod->dwOfs)
                {
                    case DIMOFS_X: 
                        mousx += lpdidod->dwData; 
                    break;
                    case DIMOFS_Y: 
                        mousy += lpdidod->dwData; 
                    break;
                    case DIMOFS_Z: 
                        mousz += lpdidod->dwData; 
                    break;
                    case DIMOFS_BUTTON0: 
                        if(lpdidod->dwData&128) 
                        {
                            ext_mbstatus[0] = 1|2; 
                        }
                        else 
                        {
                            ext_mbstatus[0] &= 2;
                        }
                        gbstatus = ((gbstatus&~1)|((lpdidod->dwData>>7)&1)); 
                        moult[0] = -1; 
                        break;
                    /*case DIMOFS_BUTTON1: if (lpdidod->dwData&128) ext_mbstatus[1] = 1|2; else ext_mbstatus[1] &= 2;
                                            gbstatus = ((gbstatus&~2)|((lpdidod->dwData>>6)&2)); moult[0] = -1; break;
                    */
                    case DIMOFS_BUTTON1:
                                if(lpdidod->dwData&128) 
                                {
                                    ext_mbstatus[1] = 1 | 2;
                                } 
                                else 
                                {
                                    ext_mbstatus[1] &= ~2;
                                }
                                gbstatus = (gbstatus & ~2) | ((lpdidod->dwData >> 6) & 2);
                                moult[0] = -1;
                    break;
                    case DIMOFS_BUTTON2: 
                                if(lpdidod->dwData&128) 
                                {
                                    ext_mbstatus[2] = 1|2; 
                                }
                                else 
                                {
                                    ext_mbstatus[2] &= 2;
                                }           
                                gbstatus = ((gbstatus&~4)|((lpdidod->dwData>>5)&4)); 
                                moult[0] = -1; 
                    break;
                    case DIMOFS_BUTTON3: 
                                if(lpdidod->dwData&128) 
                                {
                                    ext_mbstatus[3] = 1|2; 
                                }
                                else 
                                {
                                    ext_mbstatus[3] &= 2;
                                }    
                                gbstatus = ((gbstatus&~8)|((lpdidod->dwData>>4)&8)); 
                                moult[0] = -1; 
                    break;
                    case DIMOFS_BUTTON4:
                                if (lpdidod->dwData&128) 
                                {
                                    ext_mbstatus[4] = 1 | 2;
                                } 
                                else 
                                {
                                    ext_mbstatus[4] &= ~2;   // Limpa o bit de "pressed"
                                }
                                gbstatus = (gbstatus & ~16) | ((lpdidod->dwData >> 3) & 16); // Atualiza o estado no gbstatus
                                moult[0] = -1;
                    break;
            }
            i++; lpdidod = &MousBuffer[i];
        } 
        while ((i < (long)dwItems) && ((long)lpdidod->dwTimeStamp == moult[0]));

            if (moult[0] != -1)
            {
                got++;
                if ((moult[1] != -1) && (moult[2] != -1) && (moult[3] != -1))
                {
                    nlt0 = moult[0]-moult[1];
                    nlt1 = moult[1]-moult[2];
                    nlt2 = moult[2]-moult[3];
                    ltmin = nlt0; ltmax = nlt0;
                    if (nlt1 < ltmin) ltmin = nlt1;
                    if (nlt2 < ltmin) ltmin = nlt2;
                    if (nlt1 > ltmax) ltmax = nlt1;
                    if (nlt2 > ltmax) ltmax = nlt2;
                    if (ltmin*2 >= ltmax) //WARNING: NT's timer has 10ms resolution!
                    {
                        moultavg += moult[0]-moult[3]; moultavgcnt += 3;
                        mousper = (float)moultavg/(float)moultavgcnt;
                    }
                }
            }
        }
        if(gkillbstatus) 
        { 
            gkillbstatus = 0; 
            gbstatus = 0; 
        } //Flush packets after task switch
    
        (*bstatus) = gbstatus;

            //Calculate and return smoothed mouse data in: (fmousx, fmousy)
        odmoutsc = dmoutsc; 
        readklock(&dmoutsc);
        fmousynctics = (float)((dmoutsc-odmoutsc)*1000.0);

            //At one time, readklock() wasn't always returning increasing values.
            //This made fmousynctics <= 0 possible, causing /0. Fixed now :)
        if((!moultavgcnt) || (!mousmoth)) //|| ((*(long *)&fmousynctics) <= 0))
        { 
            (*fmousx) = (float)mousx; 
            (*fmousy) = (float)mousy; 
            (*fmousz) = (float)mousz; 
            return; 
        }

        mousince = min(mousince+mousper*(float)got,mousper+fmousynctics);
        if(fmousynctics >= mousince) 
        { 
            f = 1; mousince = 0; 
        }
        else 
        { 
            f = fmousynctics / mousince; 
            mousince -= fmousynctics; 
        }
        mougoalx += (float)mousx; (*fmousx) = mougoalx*f; mougoalx -= (*fmousx);
        mougoaly += (float)mousy; (*fmousy) = mougoaly*f; mougoaly -= (*fmousy);
        mougoalz += (float)mousz; (*fmousz) = mougoalz*f; mougoalz -= (*fmousz);
    }

//DirectInput (KEYBOARD) VARIABLES & CODE-------------------------------------------------------
static long dinputkeyboardflags = DISCL_NONEXCLUSIVE|DISCL_FOREGROUND;

static LPDIRECTINPUTDEVICE8 gpKeyboard = 0;
#define KBDBUFFERSIZE 64
static DIDEVICEOBJECTDATA KbdBuffer[KBDBUFFERSIZE];

//#define KEYBUFSIZ 256
//static long keybuf[KEYBUFSIZ], keybufr = 0, keybufw = 0, keybufw2 = 0;

static 
void uninitkeyboard()
{
    if(gpKeyboard)
    {
        if(dinputevent[1])
        {
            gpKeyboard->lpVtbl->SetEventNotification(gpKeyboard, dinputevent[1]);
            CloseHandle(dinputevent[1]); dinputevent[1] = 0;
        }
        gpKeyboard->lpVtbl->Unacquire(gpKeyboard); 
        gpKeyboard->lpVtbl->Release(gpKeyboard); 
        gpKeyboard = 0;
    }
}

static 
long initkeyboard(HWND hwnd)
{
    HRESULT hr;
    DIPROPDWORD dipdw;
    char buf[256];

    if((hr = gpdi->lpVtbl->CreateDevice(gpdi, &GUID_SysKeyboard, &gpKeyboard,0)) < 0) 
    {
        goto initkeyboard_bad;
    }
    
    if((hr = gpKeyboard->lpVtbl->SetDataFormat(gpKeyboard, &c_dfDIKeyboard)) < 0) 
    {
        goto initkeyboard_bad;
    }
    
    if((hr = gpKeyboard->lpVtbl->SetCooperativeLevel(gpKeyboard, hwnd, dinputkeyboardflags)) < 0) 
    {
        goto initkeyboard_bad;
    }

    dinputevent[1] = CreateEvent(0,0,0,0); 

    if(!dinputevent[1])
    {
        goto initkeyboard_bad;
    }
    
    if((hr = gpKeyboard->lpVtbl->SetEventNotification(gpKeyboard, dinputevent[1])) < 0) 
    {
        goto initkeyboard_bad;
    }

    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = KBDBUFFERSIZE;
    
    if((hr = gpKeyboard->lpVtbl->SetProperty(gpKeyboard, DIPROP_BUFFERSIZE, &dipdw.diph)) < 0) 
    {
        goto initkeyboard_bad;
    }
    
    if(kbd_acquire)
    { 
        gpKeyboard->lpVtbl->Acquire(gpKeyboard); 
        shkeystatus = 0; 
    }
    return(1);

initkeyboard_bad:;
    uninitkeyboard();
    wsprintf(buf,"initdirectinput(keyboard) failed: %08lx\n",hr);
    MessageBox(hwnd, buf, "ERROR", MB_OK);
    return(0);
}

static 
long readkeyboard()
{
    HRESULT hr;
    long i;
    unsigned long dwItems;
    DIDEVICEOBJECTDATA *lpdidod;

    dwItems = KBDBUFFERSIZE;
    hr = gpKeyboard->lpVtbl->GetDeviceData(gpKeyboard, sizeof(DIDEVICEOBJECTDATA), KbdBuffer, &dwItems,0);
    //if (hr == DI_BUFFEROVERFLOW) ?;
    if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
    {
        gpKeyboard->lpVtbl->Acquire(gpKeyboard); 
        shkeystatus = 0;
        hr = gpKeyboard->lpVtbl->GetDeviceData(gpKeyboard, sizeof(DIDEVICEOBJECTDATA),KbdBuffer,&dwItems,0);
    }

    if(hr < 0) 
    {
        return(0);
    }
    
    for(i=0;i<(long)dwItems;i++)
    {
        lpdidod = &KbdBuffer[i];

        if(lpdidod->dwData&128) 
        {
            keystatus[lpdidod->dwOfs] = 1;
        }                         
        else 
        {
            keystatus[lpdidod->dwOfs] = 0;
        }

        //event occured "GetTickCount()-lpdidod->dwTimeStamp" milliseconds ago

        // +TD:
        if(lpdidod->dwData&128) 
        {
            ext_keystatus[lpdidod->dwOfs] = 1|2;
        }
        else 
        {
            ext_keystatus[lpdidod->dwOfs] &= ~1; // preserve bit 2 only
        }
    }
    return(dwItems);
}

static 
DWORD keyInputDInput(int keyCode, InputMode mode) 
{
    static DWORD keyPressTimes[256] = {0}; // Armazena os tempos de pressionamento
    static DWORD keyReleaseTimes[256] = {0}; // Armazena os tempos de soltura
    static char prevKeystatus[256] = {0}; // Armazena o estado anterior de cada tecla

    // Atualiza o estado do teclado
    readkeyboard();

    // Lógica de entrada para SINGLE
    if(mode == SINGLE) 
    {
        if(keystatus[keyCode] && !prevKeystatus[keyCode]) 
        {
            prevKeystatus[keyCode] = 1; // Marca como processado
            return 1; // Tecla pressionada pela primeira vez
        }
        if (!keystatus[keyCode]) 
        {
            prevKeystatus[keyCode] = 0; // Reset ao soltar
        }
    }

    // Lógica de entrada para KEEP
    if (mode == KEEP) 
    {
        return keystatus[keyCode];
    }

    // Lógica de entrada para KEEP_TIME
    if (mode == KEEP_TIME) 
    {
        if (keystatus[keyCode] && !prevKeystatus[keyCode]) 
        {
            keyPressTimes[keyCode] = GetTickCount(); // Registra o tempo de início
            prevKeystatus[keyCode] = 1; // Marca como pressionado
        }
        if (!keystatus[keyCode] && prevKeystatus[keyCode]) 
        {
            keyReleaseTimes[keyCode] = GetTickCount(); // Registra o tempo de soltura
            prevKeystatus[keyCode] = 0; // Reset
            return keyReleaseTimes[keyCode] - keyPressTimes[keyCode]; // Retorna a duração
        }
    }

    return 0; // Nenhum evento detectado
}

//============================================
// Joysticks / testando com o dualshock 4 e 5

    // Suporte para até 8 joysticks, não acho que mais que isso é necessario
static LPDIRECTINPUTDEVICE8 gpJoysticks[8] = {0}; 

    // não acho que alguem vai trabalhar com o joy fora da instancia
static DWORD joystickFlags = DISCL_EXCLUSIVE | DISCL_FOREGROUND;

static DIDEVCAPS joystickCapabilities;

// Estrutura e variáveis globais para contexto
typedef struct 
{
    int joystickIndex; // Índice do joystick a ser configurado
    LPDIRECTINPUTDEVICE8 *gpJoysticks; // Array de dispositivos joystick
} JoystickContext;

 //pre definicao de funcao
static void uninitJoystick(int joystickIndex);

static BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCEA* pdidInstance, VOID* pContext) 
{
    JoystickContext *ctx = (JoystickContext*)pContext;

    if (ctx->joystickIndex <= 0) 
    {
        HRESULT hr = gpdi->lpVtbl->CreateDevice(gpdi,
                                                &pdidInstance->guidInstance,
                                                &ctx->gpJoysticks[ctx->joystickIndex],
                                                NULL);

        if (FAILED(hr)) 
        {
            return DIENUM_STOP; // Erro na criação do dispositivo
        }

        return DIENUM_STOP; // Dispositivo configurado
    }

    ctx->joystickIndex--; // Decrementa o índice para próximo dispositivo
    return DIENUM_CONTINUE; // Continua a busca por dispositivos
}


static long initJoystick(HWND hwnd, int joystickIndex) 
{
    HRESULT hr;
    DIPROPDWORD dipdw;
    JoystickContext ctx;
    ctx.joystickIndex = joystickIndex;
    ctx.gpJoysticks = gpJoysticks;

    if(joystickIndex >= 8 || gpJoysticks[joystickIndex]) 
    {
        printf("\n [ !ERROR ]: initJoystick() -> indice invalido ou joystick ja iniciado \n");
        return 0; // Índice inválido ou joystick já inicializado
    }

    ctx.joystickIndex = joystickIndex;
    ctx.gpJoysticks = gpJoysticks;

    // Enumera dispositivos conectados
    hr = gpdi->lpVtbl->EnumDevices(gpdi,
                                  DI8DEVCLASS_GAMECTRL,
                                  EnumJoysticksCallback,
                                  &ctx,
                                  DIEDFL_ATTACHEDONLY);

    if(FAILED(hr) || !gpJoysticks[joystickIndex]) 
    {
        printf("\n [ !ERROR ]: initJoystick() -> Falha na enumeraco ou joystick nao encontrado \n");
        uninitJoystick(joystickIndex); // Garante limpeza em erro
        return 0; // Falha na enumeração ou joystick não encontrado
    }

    // Configurar formato de dados
    hr = gpJoysticks[joystickIndex]->lpVtbl->SetDataFormat(gpJoysticks[joystickIndex], 
                                                           &c_dfDIJoystick2);

    if (FAILED(hr)) 
    {
        printf("\n [ !ERROR ]: initJoystick() -> Falha ao configurar formato \n");
        return 0; // Falha ao configurar formato
    }

    // Configurar nível de cooperação
    hr = gpJoysticks[joystickIndex]->lpVtbl->SetCooperativeLevel(gpJoysticks[joystickIndex], 
                                                                hwnd, 
                                                                joystickFlags);

    if (FAILED(hr)) 
    {
        printf("\n [ !ERROR ]: initJoystick() -> Falha ao configurar nível de cooperação \n");
        return 0; // Falha ao configurar nível de cooperação
    }

    // Obter capacidades do dispositivo
    joystickCapabilities.dwSize = sizeof(DIDEVCAPS);
    gpJoysticks[joystickIndex]->lpVtbl->GetCapabilities(gpJoysticks[joystickIndex], 
                                                        &joystickCapabilities);

    // Configurar buffer de entrada
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = 16; // Buffer para 16 eventos

    gpJoysticks[joystickIndex]->lpVtbl->SetProperty(gpJoysticks[joystickIndex], 
                                                    DIPROP_BUFFERSIZE, 
                                                    &dipdw.diph);

    // Adquirir controle do joystick
    gpJoysticks[joystickIndex]->lpVtbl->Acquire(gpJoysticks[joystickIndex]);
    
    return 1; // Sucesso
}

// Ler estado de um joystick
static long readJoystick(int joystickIndex, DIJOYSTATE2* joyState) 
{
    HRESULT hr;

    if (joystickIndex >= 8 || !gpJoysticks[joystickIndex]) 
    {
        return 0; // Indice invalido ou dispositivo inexistente
    }

    hr = gpJoysticks[joystickIndex]->lpVtbl->Poll(gpJoysticks[joystickIndex]);
    if(FAILED(hr)) 
    {
        gpJoysticks[joystickIndex]->lpVtbl->Acquire(gpJoysticks[joystickIndex]);
        hr = gpJoysticks[joystickIndex]->lpVtbl->Poll(gpJoysticks[joystickIndex]);
    }

    if(FAILED(hr)) 
    {
        return 0; // Falha ao acessar estado
    }


    hr = gpJoysticks[joystickIndex]->lpVtbl->GetDeviceState(gpJoysticks[joystickIndex],
                                                            sizeof(DIJOYSTATE2),
                                                            joyState);

    return(SUCCEEDED(hr));
}




// Uninitialize joystick
static void uninitJoystick(int joystickIndex) 
{
    if (joystickIndex < 8 && gpJoysticks[joystickIndex]) 
    {
        gpJoysticks[joystickIndex]->lpVtbl->Unacquire(gpJoysticks[joystickIndex]);
        gpJoysticks[joystickIndex]->lpVtbl->Release(gpJoysticks[joystickIndex]);
        gpJoysticks[joystickIndex] = NULL;
    }
}

// Função principal para inicializar múltiplos joysticks
static long initJoysticks(HWND hwnd) 
{
    int i;
    for (i = 0; i < 8; i++) 
    {
        if (!initJoystick(hwnd, i)) 
        {
            break; // Para quando não encontrar mais joysticks
        }
    }
    return 1;
}

// Desconectar todos os joysticks
static void uninitAllJoysticks() 
{
    int i;
    for(i = 0; i < 8; i++) 
    {
        uninitJoystick(i);
    }
}



/*
static 
DWORD keyInputDInput(int keyCode, InputMode mode) 
{
    static DWORD keyPressTimes[256] = {0}; // Armazena os tempos de pressionamento
    static DWORD keyReleaseTimes[256] = {0}; // Armazena os tempos de soltura
    static char prevKeystatus[256] = {0}; // Armazena o estado anterior de cada tecla

    // Atualiza o estado do teclado
    readkeyboard();

    // Lógica de entrada para SINGLE
    if(mode == SINGLE) 
    {
        if(keystatus[keyCode] && !prevKeystatus[keyCode]) 
        {
            prevKeystatus[keyCode] = 1; // Marca como processado
            return 1; // Tecla pressionada pela primeira vez
        }
        if (!keystatus[keyCode]) 
        {
            prevKeystatus[keyCode] = 0; // Reset ao soltar
        }
    }

    // Lógica de entrada para KEEP
    if (mode == KEEP) 
    {
        return keystatus[keyCode];
    }

    // Lógica de entrada para KEEP_TIME
    if (mode == KEEP_TIME) 
    {
        if (keystatus[keyCode] && !prevKeystatus[keyCode]) 
        {
            keyPressTimes[keyCode] = GetTickCount(); // Registra o tempo de início
            prevKeystatus[keyCode] = 1; // Marca como pressionado
        }
        if (!keystatus[keyCode] && prevKeystatus[keyCode]) 
        {
            keyReleaseTimes[keyCode] = GetTickCount(); // Registra o tempo de soltura
            prevKeystatus[keyCode] = 0; // Reset
            return keyReleaseTimes[keyCode] - keyPressTimes[keyCode]; // Retorna a duração
        }
    }

    return 0; // Nenhum evento detectado
}
*/


#endif /* INPUT_MANAGER_H */
