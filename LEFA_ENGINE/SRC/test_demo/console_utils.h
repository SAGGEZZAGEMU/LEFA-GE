#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#define ANSI_COLOR_RED     "\x1b[31m" 
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

/*
// Cores padrão
typedef enum {
    LE_COLOR_DEFAULT = 7,  // Texto branco, fundo preto
    LE_COLOR_RED = FOREGROUND_RED | FOREGROUND_INTENSITY,
    LE_COLOR_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    LE_COLOR_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    LE_COLOR_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    LE_COLOR_CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    LE_COLOR_MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    LE_COLOR_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    LE_COLOR_BLACK = 0
} LE_ConsoleColor;
*/

/*
typedef enum {
    LE_COLOR_DEFAULT = 7, // Branco (padrão)
    LE_COLOR_RED = 4,
    LE_COLOR_GREEN = 2,
    LE_COLOR_BLUE = 1,
    // Outras cores podem ser adicionadas
} LE_ConsoleColor;
*/

typedef enum {
    LE_COLOR_DEFAULT = 7,  // Texto branco, fundo preto
    LE_COLOR_RED = FOREGROUND_RED | FOREGROUND_INTENSITY, // Vermelho
    LE_COLOR_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY, // Verde
    LE_COLOR_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY, // Azul
    LE_COLOR_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, // Amarelo
    LE_COLOR_CYAN = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // Ciano
    LE_COLOR_MAGENTA = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // Magenta
    LE_COLOR_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, // Branco Intenso
    LE_COLOR_BLACK = 0 // Preto
} LE_ConsoleColor;

// Enum para codificação de texto
typedef enum {
    ANSI = 0,  // Padrão (ASCII/ANSI)
    UTF8 = 1,  // Suporte a UTF-8
} LE_TextEncoding;


/*
// Inicializa o console para Unicode e cores
void LE_initConsole() 
{
    // Habilitar modo de processamento para cores ANSI e Unicode
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    if (GetConsoleMode(hConsole, &mode)) 
    {
        SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT);
    }

    // Configurar código de página UTF-8
    SetConsoleOutputCP(CP_UTF8);
}

void enableANSI() 
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;

    system("chcp 65001 > nul");

    // Obtém o modo atual do terminal
    if (GetConsoleMode(hOut, &dwMode)) 
    {
        // Habilita o processamento de sequência ANSI
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    } 
    else 
    {
        printf("Erro ao acessar o console.\n");
    }
}

*/

void LE_initConsole() 
{
    // Configurar o código de página UTF-8 para saída Unicode
    SetConsoleOutputCP(CP_UTF8);
}


/*
// Define a cor do texto no console
void LE_setColor(LE_ConsoleColor color) 
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
*/

void LE_setColor(LE_ConsoleColor color) 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


/* Usos da função LE_print();
		
	Uso Básico: LE_print("Hello, world!\n");
	funciona como print simples.
	
	Com cor: LE_print(LE_COLOR_RED, "Texto em vermelho!\n");
	aplica uma cor selecionada pelo parametro ao texto.
	
	Com UTF-8: LE_print(UTF8, "Unicode : \u2603\n");
	aplica o suporte a UTF-8 ao texto. 
	Indisponivel para sistemas mais antigos anteriores ao windows 10.
	
	Com cor e UTF-8: LE_print(UTF8, LE_COLOR_GREEN, "Unicode e verde: \u2603\n");
	aplica o suporte a UTF-8 com coloração de parametro.
	
	Com Variaveis: int valor = 42; LE_print(LE_COLOR_YELLOW, "Valor: %d\n", valor);

	Esses são alguns exemplos de como usar essa função.
	

 */

// Função LE_print com suporte simplificado e modular
// Função principal LE_print
void LE_print(int firstArg, ...) 
{
	va_list args;
	HANDLE hConsole;
	DWORD written;
	char buffer[1024];
    wchar_t wideBuffer[1024];
	LE_ConsoleColor color;
	int encoding;
	const char *format;
	int currentArg;
    
	
	va_start(args, firstArg);

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    color = LE_COLOR_DEFAULT;
    encoding = ANSI;
    format = NULL;

    // Interpretar os argumentos
    currentArg = firstArg;
    while (currentArg != 0) 
	{
        if (currentArg >= LE_COLOR_DEFAULT && currentArg <= LE_COLOR_YELLOW) 
		{
            color = (LE_ConsoleColor)currentArg; // Detectar cor
        } 
		else if (currentArg == UTF8) 
		{
            encoding = UTF8; // Detectar UTF-8
        } 
		else 
		{
            format = (const char *)currentArg; // Detectar string formatada
            break;
        }
        currentArg = va_arg(args, int);
    }

    // Configurar cor
    LE_setColor(color);

    // Se não encontrou formato, encerra
    if(!format) 
	{
        va_end(args);
        return;
    }

    // Construir string formatada
    vsnprintf(buffer, sizeof(buffer), format, args);

    if(encoding == UTF8) 
	{
        // Converter para Unicode (UTF-16) e usar WriteConsoleW
        MultiByteToWideChar(CP_UTF8, 0, buffer, -1, wideBuffer, sizeof(wideBuffer) / sizeof(wchar_t));
        WriteConsoleW(hConsole, wideBuffer, wcslen(wideBuffer), &written, NULL);
    } 
	else 
	{
        // Usar WriteConsoleA para saída normal
        WriteConsoleA(hConsole, buffer, strlen(buffer), &written, NULL);
    }

    // Resetar cor
    LE_setColor(LE_COLOR_DEFAULT);

    va_end(args);
}

/*
// Função principal LE_print
void LE_print(LE_ConsoleColor color, const wchar_t *format, ...) 
{
    va_list args;
    UINT codePage;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    wchar_t buffer[1024];

    // Configurar a cor
    LE_setColor(color);

    // Processar argumentos variáveis
    va_start(args, format);
    vswprintf(buffer, sizeof(buffer) / sizeof(wchar_t), format, args);
    va_end(args);

    // Obter o código de página atual
    codePage = GetConsoleOutputCP();

    // Se estiver em UTF-8, usar WriteConsoleW diretamente
    if (codePage == CP_UTF8) 
    {
        WriteConsoleW(hConsole, buffer, wcslen(buffer), &written, NULL);
    } else 
    {
        // Caso contrário, converter para o código de página atual
        char convertedBuffer[1024];
        WideCharToMultiByte(codePage, 0, buffer, -1, convertedBuffer, sizeof(convertedBuffer), NULL, NULL);
        WriteConsoleA(hConsole, convertedBuffer, strlen(convertedBuffer), &written, NULL);
    }

    // Resetar a cor para o padrão
    LE_setColor(LE_COLOR_DEFAULT);
}
*/


/*void LE_print(LE_ConsoleColor color, const wchar_t *format, ...) 
{
    va_list args;
    wchar_t buffer[1024];
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;

    // Configurar a cor
    LE_setColor(color);

    // Processar argumentos variáveis
    va_start(args, format);
    vswprintf(buffer, sizeof(buffer) / sizeof(wchar_t), format, args);
    va_end(args);

    // Escrever no console
    WriteConsoleW(hConsole, buffer, wcslen(buffer), &written, NULL);

    // Resetar a cor para o padrão
    LE_setColor(LE_COLOR_DEFAULT);
}*/


/* just to write an error message */
static void LE_printERR(const char *errorMsg) 
{
    printf("[ ERROR ] : %s\n", errorMsg);
}


/* This function is for writing a line without having to skip */
static void LE_printLine(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    printf("\n");
    va_end(args);
}





/* this is a engine version */
#define ev_rel "20.01.24" /* If you change this it won't change anything.
                             It's just to have good control */
static void engineVersion()
{
    LE_printLine("LEFA : %s", ev_rel );
}


#endif /* CONSOLE_UTILS_H */
