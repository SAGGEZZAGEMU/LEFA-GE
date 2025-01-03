@echo off 

REM LIPS OS SUNA MODEL TEST

set COMPILATION_ARGS=/6r -fp6 -e600

set CC=wcc386
set EXIT=../bin/test_demo

cd SRC
echo.
echo.
@wcc386 /6r -fp6  -e500 -i="lefa/include" "test_demo/main.c" -fo="../bin/test_demo/"
echo.
echo.
echo.
echo.
@wcc386 /6r -fp6 -i="lefa/include" "lefa/c_src/platforms/windows/windowSystem.c" -fo="../bin/test_demo/"
echo.
echo.
echo.
echo.
@wcc386 /6r -fp6 -i="lefa/include" "lefa/c_src/render/api_manager/gl_manager/gl_manager.c" -fo="../bin/test_demo/"
echo.
echo.
echo GLAD_OPENGL COMPILATIOn
echo.
echo.
@wcc386 /6r -fp6 -i="lefa/include" "lefa/c_src/3rdparty/glad/OpenGL/glad_gl.c" -fo="../bin/test_demo/"
echo.
echo.








	cd ..
echo.
echo.
@wlink name test_demo.exe ^
		file bin/test_demo/main.obj, ^
	 		bin/test_demo/windowSystem.obj, ^
	 		bin/test_demo/gl_manager.obj, ^
	 		bin/test_demo/glad_gl.obj, ^
		Library opengl32.lib, ^
			lib/pthreadWC2.lib, ^
			lib/zlib.lib, ^
			lib/libiconv.lib
echo.
echo.


move test_demo.exe bin/test_demo/












pause 