#include"vk_pipeline.h"

// vk_pipeline.c
#include"vk_pipeline.h"
#include<stdio.h>
#include<stdlib.h>

VkShaderModule createShaderModule(VkDevice device, const char* filePath) 
{
    FILE* file = fopen(filePath, "rb");
    VkShaderModuleCreateInfo createInfo;
    VkShaderModule shaderModule;
    long fileSize;
    char* shaderCode;

    if(!file) 
    {
        fprintf(stderr, "Falha ao abrir o arquivo de shader: %s\n", filePath);
        return VK_NULL_HANDLE;
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    shaderCode = malloc(fileSize);
    fread(shaderCode, 1, fileSize, file);
    fclose(file);


    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = fileSize;
    createInfo.pCode = (const uint32_t*)shaderCode;

    if (vkCreateShaderModule(device, &createInfo, NULL, &shaderModule) != VK_SUCCESS) 
    {
        fprintf(stderr, "Falha ao criar o módulo de shader\n");
        shaderModule = VK_NULL_HANDLE;
    }
    free(shaderCode);
    return shaderModule;
}

VkPipeline createGraphicsPipeline(VkDevice device, VkRenderPass renderPass) 
{
    // Configuração fictícia do pipeline para exemplo
    return VK_NULL_HANDLE;
}









