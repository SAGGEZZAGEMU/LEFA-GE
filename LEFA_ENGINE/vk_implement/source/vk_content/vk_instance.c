#include"vk_instance.h"

#include<stdio.h>
#include<string.h>

VkInstance createVkInstance(const char* appName, const char* engineName) 
{
    VkInstance instance;
    VkApplicationInfo appInfo;
    VkInstanceCreateInfo createInfo;


    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = engineName;
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    memset(&createInfo, 0, sizeof(VkInstanceCreateInfo));
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    if (vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS) 
    {
        fprintf(stderr, "Falha ao criar a instância Vulkan\n");
    }
    return instance;
}

void destroyVkInstance(VkInstance instance) 
{
    vkDestroyInstance(instance, NULL);
}













