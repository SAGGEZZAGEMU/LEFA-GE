#ifndef UTILS_H
#define UTILS_H

#ifdef APIENTRY
	#undef APIENTRY
#endif

#include <windows.h>

#include <stdlib.h>
#include <time.h>
#include <stdint.h> // Para uint64_t


/* Função que retorna um número aleatório do tipo double */
double Utils_Rand()
{
    int x = rand();
    double f = (double)x / RAND_MAX;
    return f;
}

/* Função que retorna um número inteiro aleatório no intervalo [0, modulo) */
int Utils_RandInt(int modulo)
{
    int x = rand() % modulo;
    return x;
}

/* Função que define a semente do gerador de números aleatórios */
void Utils_SeedRand(int seed)
{
    srand((unsigned int)seed);
}

/* Função que retorna o tempo em microssegundos desde a época */
uint64_t Utils_GetMicroSeconds()
{
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency); // Obtém a frequência do contador de alta resolução
    QueryPerformanceCounter(&counter);    // Obtém o valor atual do contador
    return ((uint64_t)counter.QuadPart * 1000000) / frequency.QuadPart; // Converte para microssegundos
}

#endif
