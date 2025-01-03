
#include"window_manager.h"

#include"vk_content/vk_buffers.h"
#include"vk_content/vk_instance.h"
#include"vk_content/vk_pipeline.h"
#include"vk_content/vk_render.h"
#include"vk_content/vk_surface.h"
#include"vk_content/vk_utils.h"
#include"vk_content/vk_global.h"

#include<stdio.h>

int activeProgram = 1;

LE_Window *window1;


void update()
{

}

int main()
{
	window1 = engineCreateDisplay(" Vulkan Test ", 800, 600);


	engineSetLoop(window1, &activeProgram, update);


	return 0;
}





