#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

typedef struct 
{
    void *data;      // Ponteiro para os dados (qualquer tipo de dado)
    int size;        // Número de elementos atualmente no array
    int capacity;    // Capacidade total do array
    size_t element_size; // Tamanho de cada elemento armazenado
}DynamicArray;

void initDynamicArray(DynamicArray *array, 
					  int initialCapacity, 
					  size_t element_size) 
{
    array->data = malloc(initialCapacity * element_size);  // Aloca memória para os elementos
    array->size = 0;
    array->capacity = initialCapacity;
    array->element_size = element_size;  // Armazena o tamanho do tipo de dado
}

void pushBack(DynamicArray *array, void *value) 
{
    if (array->size >= array->capacity) 
    {
        array->capacity *= 2;  // Dobra a capacidade
        array->data = realloc(array->data, array->capacity * array->element_size);
    }
    // Copia o valor para o array, considerando o tamanho do elemento
    memcpy((char*)array->data + (array->size * array->element_size), value, array->element_size);
    array->size++;
}

void* get(DynamicArray *array, 
	      int index) 
{
    if (index >= 0 && index < array->size) 
    {
        return (char*)array->data + (index * array->element_size);  // Retorna um ponteiro para o elemento
    }
    return NULL;  // Índice inválido
}

void freeDynamicArray(DynamicArray *array) 
{
    free(array->data);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
}












#endif /* DYNAMICARRAY_H */
