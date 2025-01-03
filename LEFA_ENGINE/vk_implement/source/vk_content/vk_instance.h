/* vk_instance.h */
/*
	Funções para criar e destruir a instância Vulkan.
	Configuração de camadas de validação e extensões.
	Seleção de dispositivo físico e criação de dispositivo lógico.
*/
#ifndef VK_INSTANCE_H
#define VK_INSTANCE_H


#include <vulkan/vulkan.h>

// Função para inicializar a instância Vulkan
VkInstance createVkInstance(const char* appName, const char* engineName);

// Função para destruir a instância Vulkan
void destroyVkInstance(VkInstance instance);

#endif


