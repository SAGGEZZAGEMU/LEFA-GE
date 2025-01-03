

typedef struct
{
	VkInstance 			instance;
	VkSurfaceKHR        surface;
	VkPhysicalDevice    gpu;
	VkDevice            device;
    VkQueue             graphics_queue;
    VkQueue             present_queue;

} LE_VK_GLOBAL;

