/* vk_pipeline.h */
/*
	Funções para criar o pipeline gráfico.
	Carregamento e compilação de shaders.
	Configuração de estados do pipeline.
*/

#ifndef VK_PIPELINE_H
#define VK_PIPELINE_H

#include<vulkan/vulkan.h>

// Função para criar um pipeline gráfico
VkPipeline createGraphicsPipeline(VkDevice device, VkRenderPass renderPass);

// Função para carregar shaders SPIR-V
VkShaderModule createShaderModule(VkDevice device, const char* filePath);

#endif

