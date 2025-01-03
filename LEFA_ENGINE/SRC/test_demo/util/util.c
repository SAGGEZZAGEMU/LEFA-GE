//#include <GL/glew.h>
#define GL_VERSION_LIMIT
#define GL2_0
#include"render/api_manager/gl_manager.h"

#include"3rdparty/lodepng/lodepng.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Boring, non-OpenGL-related utility functions
 */

/** *
file_contents() 

    * lê o conteúdo de um arquivo e o carrega em um buffer na memória. 
    * Ela é frequentemente utilizada para ler arquivos de texto, 
        * como shaders GLSL, 
            * e retornar seus dados para serem utilizados na aplicação.
    ** ======= **
    * Protótipo:
        * filename: Nome do arquivo que será lido.
        * length: Ponteiro para uma variável que armazenará o 
                            * tamanho do arquivo lido, em bytes.
    ** =========== **
    * Passo a Passo:
        * fopen(filename, "r"):
            * Abre o arquivo especificado em modo de leitura ("r").
            *Se a abertura falhar, 
                * imprime uma mensagem de erro e retorna NULL.

        * fseek(f, 0, SEEK_END):
            * Move o cursor de leitura para o final do 
                        * arquivo para determinar seu tamanho.

        * ftell(f):
            * Retorna a posição atual do cursor, 
                * que representa o tamanho total do arquivo em bytes.
            * Esse valor é armazenado em *length.

        * fseek(f, 0, SEEK_SET):
            * Move o cursor de leitura de volta para o início do arquivo.

        * malloc(*length + 1):
            * Aloca memória suficiente para armazenar o conteúdo do arquivo, 
                * com espaço adicional para um caractere '\0' no final (necessário para strings em C).

        * fread(buffer, 1, *length, f):
            * Lê o conteúdo do arquivo e o armazena no buffer alocado.
            * O valor retornado por fread é a quantidade de bytes lidos, 
                                                * que é atualizada em *length.

        * fclose(f):
            * Fecha o arquivo aberto.

        * ((char*)buffer)[*length] = '\0':
            * Adiciona o caractere nulo ('\0') ao final do buffer, 
             * garantindo que o conteúdo seja tratado como uma string válida em C.

        * return buffer:
            * Retorna o ponteiro para o buffer que contém o conteúdo do arquivo.

** ============ **
* Exemplo de Uso:
    ```
    GLint length;
    char *shader_source = file_contents("vertex_shader.glsl", &length);

    if (shader_source) 
    {
        printf("Conteúdo do arquivo:\n%s\n", shader_source);
        free(shader_source);  // Não esqueça de liberar a memória após o uso
    }
    ```
** ================ **
* O Que a Função Faz:
    * Abre um arquivo de texto.
    * Lê seu conteúdo na memória.
    * Retorna o conteúdo como uma string, juntamente com seu tamanho.

*/

void *file_contents(const char *filename, GLint *length)
{
    FILE *f = fopen(filename, "r");
    void *buffer;

    if(!f) 
    {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    if (*length == 0) 
    {
        fclose(f);
        return NULL;
    }
    *length = ftell(f);

    fseek(f, 0, SEEK_SET);

    buffer = malloc(*length+1);
    if(!buffer) 
    {
        fprintf(stderr, "Falha ao alocar memória\n");
        fclose(f);
        return NULL;
    }

    *length = fread(buffer, 1, *length, f);
    fclose(f);
    ((char*)buffer)[*length] = '\0';

    return buffer;
}

/** * 
le_short() 

    * lê dois bytes de uma sequência de bytes e os interpreta como 
        * um inteiro de 16 bits com sinal no formato little-endian.

** ======= **
* Protótipo
    * bytes: Ponteiro para um array de dois bytes (unsigned char), 
        * representando o número de 16 bits em formato little-endian.

** ===================== **
* Funcionamento da Função:
    * bytes[0]:
        * Representa o LSB (byte menos significativo), que será usado diretamente.

    * (char)bytes[1] << 8:
        * Converte o MSB (byte mais significativo) para char (mantendo o sinal), 
            * e o desloca 8 bits para a esquerda, 
                * para ocupar a posição correta no número final de 16 bits.

    * bytes[0] | ((char)bytes[1] << 8):
        * Faz a operação OR bit a bit entre o LSB e o MSB deslocado, 
                                * combinando-os em um único valor short.

** ======================== **
* Exemplo de Entrada e Saída:
    ``` 
        unsigned char bytes[] = {0x34, 0x12};  // Little-endian de 0x1234

        short value = le_short(bytes);  // Resultado: 0x1234 (decimal: 4660)
    ```
    
    * Se bytes[] fosse {0xFF, 0x7F}, 
        * isso corresponderia a 0x7FFF (máximo valor positivo de um short).

** ================== **
* Detalhes Importantes:
    * Conversão de char:
        * O tipo char (quando assinado) é importante para preservar o sinal. 
            *Isso é relevante para números negativos:

            * Exemplo: {0xFF, 0xFF} (little-endian de 0xFFFF) retornaria -1 como short.

            * Alternativa sem Sinal:
                * Se você quiser ler um valor sem sinal (unsigned short), 
                                            *basta remover o cast para char:
                    ```
                    return bytes[0] | (bytes[1] << 8);  // Sem sinal
                    ```
** ============ **
* Exemplo de Uso:

    ```
        #include <stdio.h>

        static short le_short(unsigned char *bytes)
        {
            return bytes[0] | ((char)bytes[1] << 8);
        }

        int main() 
        {
            unsigned char data[] = {0x78, 0x56};  // Little-endian de 0x5678
            short value = le_short(data);
            
            printf("Valor lido: %d (hex: 0x%X)\n", value, value);  // Saída: 22136 (hex: 0x5678)
            return 0;
        }
    ```

** ======== **
* Conclusão:
    * A função le_short é útil para ler números de 16 bits armazenados no formato little-endian.
    * É frequentemente utilizada para manipular arquivos binários, 
        * como imagens TGA, 
            * arquivos WAV, 
                * ou outros formatos que armazenam dados em little-endian.

*/
static short le_short(unsigned char *bytes)
{
    return bytes[0] | ((char)bytes[1] << 8);
}



/** * 
read_tga() 
    
    * lê uma imagem no formato TGA (Truevision TGA ou TARGA) a partir de um arquivo, 
                                * e retorna os dados de pixels da imagem em um buffer. 
        * Além disso, ela preenche os parâmetros de largura e altura da imagem.

* Protótipo:
    * filename: Nome do arquivo TGA a ser lido.
      * width: Ponteiro para armazenar a largura da imagem.
      * height: Ponteiro para armazenar a altura da imagem.
      * Retorno: Um ponteiro para os dados dos pixels da imagem em formato RGB (ou NULL em caso de falha).


*/
void *read_tga(const char *filename, int *width, int *height)
{
    struct tga_header 
    {
       char  id_length;
       char  color_map_type;
       char  data_type_code;
       unsigned char  color_map_origin[2];
       unsigned char  color_map_length[2];
       char  color_map_depth;
       unsigned char  x_origin[2];
       unsigned char  y_origin[2];
       unsigned char  width[2];
       unsigned char  height[2];
       char  bits_per_pixel;
       char  image_descriptor;
    } header;


    int i, color_map_size, pixels_size;
    FILE *f;
    size_t read;
    void *pixels;

    f = fopen(filename, "rb");

    if (!f) 
    {
        fprintf(stderr, "Unable to open %s for reading\n", filename);
        return NULL;
    }

    read = fread(&header, 1, sizeof(header), f);

    if (read != sizeof(header)) 
    {
        fprintf(stderr, "%s has incomplete tga header\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.data_type_code != 2) 
    {
        fprintf(stderr, "%s is not an uncompressed RGB tga file\n", filename);
        fclose(f);
        return NULL;
    }
    if (header.bits_per_pixel != 24) 
    {
        fprintf(stderr, "%s is not a 24-bit uncompressed RGB tga file\n", filename);
        fclose(f);
        return NULL;
    }

    for (i = 0; i < header.id_length; ++i)
        if (getc(f) == EOF) 
        {
            fprintf(stderr, "%s has incomplete id string\n", filename);
            fclose(f);
            return NULL;
        }

    color_map_size = le_short(header.color_map_length) * (header.color_map_depth/8);
    for (i = 0; i < color_map_size; ++i)
        if (getc(f) == EOF) 
        {
            fprintf(stderr, "%s has incomplete color map\n", filename);
            fclose(f);
            return NULL;
        }

    *width = le_short(header.width); *height = le_short(header.height);
    pixels_size = *width * *height * (header.bits_per_pixel/8);
    pixels = malloc(pixels_size);

    read = fread(pixels, 1, pixels_size, f);
    fclose(f);

    if (read != pixels_size) 
    {
        fprintf(stderr, "%s has incomplete image\n", filename);
        free(pixels);
        return NULL;
    }

    return pixels;
}

// Carrega uma textura com LodePNG
GLuint load_texture_png(const char *filename) 
{
    unsigned char *image;
    unsigned width, height;
    GLuint texture;

    if(lodepng_decode32_file(&image, &width, &height, filename)) 
    {
        printf("Error loading texture %s\n", filename);
        return 0;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // Repetir no eixo X
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // Repetir no eixo Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(image);
    return texture;
}

#ifdef APIENTRY
    #undef APIENTRY
#endif 

#include<windows.h>

void computeFPS(double* fpsCount) /* contador de fps a cada atualização. */
{
    static LARGE_INTEGER frequency;
    static LARGE_INTEGER t0;
    static int frames = 0;
    static double fps = 0.0;
    static char titlestring[200];
    double elapsedTime;

    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    
    // Initialize frequency and t0 on the first call
    if (frames == 0)
    {
        QueryPerformanceFrequency(&frequency);
        t0 = t;
    }

    // If one second has passed, or if this is the very first frame
    elapsedTime = (double)(t.QuadPart - t0.QuadPart) / frequency.QuadPart;
    if (elapsedTime > 0.1) /* fps count time */
    {
        fps = (double)frames / elapsedTime;
        //sprintf(titlestring, "GLSL procedural shaders (%.1f FPS)", fps);
        //SetWindowText(hwnd, titlestring);
        
        t0 = t;
        frames = 0;
    }
    frames++;
    //return fps;

    *fpsCount = fps;
}

//
double getElapsedTimeInSeconds() 
{
    static LARGE_INTEGER frequency;
    static LARGE_INTEGER t0;
    static BOOL initialized = FALSE;
    LARGE_INTEGER t;

    if (!initialized) 
    {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&t0);
        initialized = TRUE;
    }

    QueryPerformanceCounter(&t);

    return (double)(t.QuadPart - t0.QuadPart) / frequency.QuadPart;
}

