@echo off


cls
echo Open Watcom Compiler ## Debug and Otimize Mode
echo x32_80387
echo.
@wcc386 /6r -fp6 -ms -bm -ol -om -op -ot -oz -zkl -zdp -zi -ot -hc -hd -hw -oh "source/main.c" -i="source/include" ^
-DVK_USE_PLATFORM_WIN32_KHR ^
-DVK_PROTOTYPES ^
-D_CRT_SECURE_NO_WARNINGS ^
-D_USE_MATH_DEFINES ^
-D_DEBUG ^
-D_WINDOWS ^
 	 2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See" 
echo.
echo.
echo.
@wcc386 /6r -fp6 -ms -bm -ol -om -op -ot -oz -zkl -zdp -zi -ot -hc -hd -hw -oh "source/windowSystem.c" -i="source/include" ^
 	 2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See" 

echo.
echo.
echo.
@wcc386 /6r -fp6 -ms -bm -ol -om -op -ot -oz -zkl -zdp -zi -ot -hc -hd -hw -oh "source/vk_content/vk_instance.c" -i="source/include" ^
 	 2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See" 

echo.
echo.
echo.
echo.
echo.
@wcc386 /6r -fp6 -ms -bm -ol -om -op -ot -oz -zkl -zdp -zi -ot -hc -hd -hw -oh "source/vk_content/vk_pipeline.c" -i="source/include" ^
 	 2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See" 

echo.
echo.

@wlink name vk_lefa_wat.exe file *.obj Library vulkan-1.lib ^
	 	 2>&1 | findstr /V /C:"Open Watcom" /C:"Portions" /C:"See"

echo.
echo.

del main.obj
del windowSystem.obj
del main.err
echo.
echo.
pause





rem pause | echo THE BATCH ENDS
