clear

echo Program Build !

echo 
echo ======== SRC/test_demo/main.c =========
echo 
gcc -std=c89 -c -I. -ISRC/lefa/include "SRC/test_demo/main.c"
echo 
echo ============ c_src/platforms/linux/windowSystem.c ============
echo 
gcc -std=c89 -c -I. -ISRC/lefa/include "SRC/lefa/c_src/platforms/linux/windowSystem.c" -lX11 -lGL -lGLX -lGLU
echo 
echo ========= c_src/render/api_manager/gl_manager/gl_manager.c ============
echo 
gcc -std=c89 -c -I. -ISRC/lefa/include "SRC/lefa/c_src/render/api_manager/gl_manager/gl_manager.c" -lX11 -lGL -lGLX -lGLU
echo 
echo ========= c_src/3rdparty/glad/OpenGL/glad_gl.c ============
echo 
gcc -std=c89 -c -I. -ISRC/lefa/include "SRC/lefa/c_src/3rdparty/glad/OpenGL/glad_gl.c" -lGL 
echo 
echo 
echo === Linker Program ! ===
echo 
gcc main.o windowSystem.o gl_manager.o glad_gl.o -o test_linux.elf -lX11 -lGL -lGLX -lGLU
echo 
echo 

rm main.o windowSystem.o gl_manager.o glad_gl.o


