#ifndef GL_COMMAND_UTILS_H
#define GL_COMMAND_UTILS_H

/*
 * Functions for creating OpenGL objects:
 */

    /** * 

    Finalidade Geral
        Criar um buffer genérico na GPU e carregá-lo com dados fornecidos pela CPU.
        Utilizado para melhorar o desempenho gráfico ao mover grandes volumes de 
                                                dados de forma eficiente para a GPU.


        parametros    
    
       (GLenum target              Especifica o tipo de buffer a ser criado. 
                                   Ex.: GL_ARRAY_BUFFER para vértices, 
                                   GL_ELEMENT_ARRAY_BUFFER para índices.
        
       (const void *buffer_data    Ponteiro para os dados que serão carregados no buffer.
        
       (GLsizei buffer_size        Tamanho em bytes dos dados a serem carregados.
            
        corpo da função

        GLuint buffer (retorno)    Retorna o identificador (handle) do buffer criado.

        glGenBuffers(1, &buffer);
                    glGenBuffers gera um identificador único (GLuint) para o buffer na GPU.
                    1: Indica que estamos criando 1 buffer.
                    &buffer: Armazena o identificador no endereço da variável buffer.

        glBindBuffer(target, buffer);
                    glBindBuffer associa o buffer recém-criado ao contexto OpenGL.
                    target: Especifica o tipo de buffer que será associado.
                            GL_ARRAY_BUFFER: Buffer que armazena posições de vértices, cores, etc.
                            GL_ELEMENT_ARRAY_BUFFER: Buffer que armazena índices para desenhar elementos.            
        
        glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
                    glBufferData envia os dados para a GPU.
                    Parâmetros:
                                target: Tipo de buffer ao qual os dados serão enviados.
                                buffer_size: Tamanho em bytes dos dados a serem carregados.
                                buffer_data: Ponteiro para os dados na CPU.
                                GL_STATIC_DRAW: Padrão de uso do buffer, 
                                                indicando que ele será escrito uma vez e lido muitas vezes.


    a função foi melhorada adicionando um parametro adicional GLenum usage, para uma melhoria de uso generalizado,
        A função make_buffer pode ser melhorada para aceitar diferentes modos de uso.

              |  GL_STATIC_DRAW   | Os dados serão definidos uma vez e usados muitas vezes (mais comum).
              |  GL_DYNAMIC_DRAW  | Os dados serão modificados frequentemente e usados muitas vezes.
              |  GL_STREAM_DRAW   | Os dados serão modificados uma vez e usados poucas vezes.

        A troca entre esses valores pode ajudar a otimizar o desempenho, 
                    dependendo da natureza dos dados que você está carregando.

        | GL_STATIC_DRAW: Ideal para dados que não mudam (vértices fixos de um modelo 3D).
        | GL_DYNAMIC_DRAW: Melhor para dados que mudam ocasionalmente (ex.: vértices animados ou posições de partículas).
        | GL_STREAM_DRAW: Usado quando os dados mudam constantemente (ex.: geometria gerada frame a frame).    




     */
static GLuint make_buffer(GLenum target,
                          const void *buffer_data,
                          GLsizei buffer_size,
                          GLenum usage) 
{
    GLuint buffer;

    glGenBuffers(1, &buffer); // Gera um buffer ID
    if(buffer == 0) 
    {
        fprintf(stderr, "Falha ao gerar o buffer.\n");
        return 0;
    }

    glBindBuffer(target, buffer); // Vincula o buffer ao alvo (GL_ARRAY_BUFFER ou GL_ELEMENT_ARRAY_BUFFER)
    glBufferData(target, buffer_size, buffer_data, usage); // Carrega os dados no buffer com a sugestão de uso
    
    return buffer; // Retorna o ID do buffer

}

/** * 
    make_texture() 

    tem como objetivo carregar uma textura a partir de um arquivo TGA e 
                                    criar um objeto de textura no OpenGL 2.0.
*/

static GLuint make_texture(const char *filename)
{
    int width, height;
    void *pixels = read_tga(filename, &width, &height);
    GLuint texture;

    if (!pixels)
    {
        return 0; // Retorna 0 se a leitura da textura falhar
    }

    glGenTextures(1, &texture);                     // Gera um ID de textura
    glBindTexture(GL_TEXTURE_2D, texture);          // Vincula a textura como 2D
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0,           /* target, level */
                 GL_RGB8,                    /* internal format */
                 width, height, 0,           /* width, height, border */
                 GL_BGR, GL_UNSIGNED_BYTE,   /* external format, type */
                 pixels);                    /* pixels */
    
    free(pixels);
    return texture;
}


/** *
show_info_log()

 * é usada para exibir logs de informações sobre objetos OpenGL, 
 *                             como shaders e programas de shaders.


    * GLuint object: O identificador do objeto OpenGL (shader ou programa) cujo log será exibido.

    * PFNGLGETSHADERIVPROC glGet__iv: Ponteiro para a função OpenGL que obtém o tamanho do log. 
    *                       Para shaders, é glGetShaderiv, 
    *                       e para programas de shader, é glGetProgramiv.

    * PFNGLGETSHADERINFOLOGPROC glGet__InfoLog: 
    * Ponteiro para a função OpenGL que recupera o log de informações. 
    * Para shaders, é glGetShaderInfoLog, e para programas de shader, é glGetProgramInfoLog.

    * glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
    * Usa a função apontada por glGet__iv para consultar o 
            * comprimento do log (GL_INFO_LOG_LENGTH) e armazena o resultado em log_length
    * Nota: Se log_length for 0, significa que não há mensagens para exibir.

    * log = malloc(log_length);
    * Aloca dinamicamente memória suficiente para armazenar o log.

    * glGet__InfoLog(object, log_length, NULL, log); Obtém o Conteúdo do Log
    * Usa a função apontada por glGet__InfoLog para recuperar o log completo e armazená-lo em log.

    * fprintf(stderr, "%s", log); Exibe o Log no Console
    * Imprime o conteúdo do log no stderr (saída de erro padrão), que geralmente é exibido no terminal.



*/

static void show_info_log(GLuint                    object,
                          PFNGLGETSHADERIVPROC      glGet__iv,
                          PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
    GLint log_length = 0;
    char *log;
    
    if (log_length > 1) // Normalmente, o log_length mínimo é 1
    {
        char *log = malloc(log_length);

        if (log)
        {
            // Obtém o log de informações
            glGet__InfoLog(object, log_length, NULL, log);
            fprintf(stderr, "%s\n", log);  // Exibe o log com uma nova linha no final
            free(log);                     // Libera a memória alocada
        }
        else
        {
            fprintf(stderr, "Erro: Falha ao alocar memória para o log de informações.\n");
        }
    }
    else
    {
        fprintf(stderr, "Nenhum log de informações disponível.\n");
    }
}

/** *
make_shader()

  * é responsável por criar, compilar e verificar shaders em OpenGL.

    * 1. Objetivo da Função:
        * A função make_shader cria e compila um shader 
            *       a partir de um arquivo de código-fonte GLSL 
            *       e retorna o identificador do shader compilado. 
            *       Se a compilação falhar, 
            *            a função exibe o log de erro e retorna 0.

    * 2. Parâmetros da Função:
        * GLenum type: O tipo de shader a ser criado. Pode ser:
                * GL_VERTEX_SHADER para shaders de vértice.
                * GL_FRAGMENT_SHADER para shaders de fragmento.
                * GL_GEOMETRY_SHADER (caso esteja usando extensões, 
                        *  mas em OpenGL 2.0 isso não é suportado).
                * const char *filename: O caminho do arquivo que contém o 
                                        * código-fonte do shader em GLSL.
    
    * 3. Fluxo da Função: 
        * GLchar *source = file_contents(filename, &length); Carrega o Código-Fonte do Shader
            * Usa a função file_contents (presumidamente definida em outro lugar) 
                    *para carregar o conteúdo do arquivo de código-fonte do shader em 
                                                    *   memória e armazená-lo em source. 
                                        * O comprimento do código é armazenado em length.
            * Nota: file_contents retorna NULL se houver falha ao abrir ou ler o arquivo.

        * Verifica se o Código-Fonte foi Carregado
            * if(!source)
              {
                return 0;  // Retorna 0 se o arquivo não pôde ser lido
              }*
        
        * shader = glCreateShader(type); Cria o Shader
            * Cria um novo objeto de shader do tipo especificado (type) e 
                                    * armazena seu identificador em shader.

        * Define o Código-Fonte no Shader 
            * glShaderSource(shader, 1, (const GLchar**)&source, &length);
                * Associa o código-fonte GLSL ao objeto de shader.

        * Libera a Memória do Código-Fonte
            * free(source);
                * Libera a memória alocada para o conteúdo do arquivo após ele ter sido associado ao shader.
    
        * Compila o Shader
            * glCompileShader(shader);
                * Compila o shader GLSL.

        * Verifica se a Compilação Foi Bem-Sucedida
            * glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
                * Consulta o estado de compilação do shader (GL_COMPILE_STATUS) 
                                        *   e armazena o resultado em shader_ok.

        * Se a Compilação Falhar
            * if(!shader_ok)
              {
                  fprintf(stderr, "Failed to compile %s:\n", filename);
                  show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
                  glDeleteShader(shader);
                  return 0;
              }
                    * Se a compilação falhar:
                        * Exibe uma mensagem de erro no terminal 
                            *informando que o shader não pôde ser compilado.
                        * Chama show_info_log para exibir o log detalhado da falha.
                        * Deleta o shader com glDeleteShader para liberar recursos.
                        * Retorna 0.
        
        * Se a Compilação For Bem-Sucedida
            * return shader;
                * Retorna o identificador do shader compilado.

    * Possibilidades Futuras:
        *Suporte a Diferentes Tipos de Shaders: Além de shaders de vértice e fragmento, 
            * a função pode ser estendida para outros tipos de shaders 
                                * se o OpenGL suportar versões mais avançadas.

        * Shader Cache: Implementar um cache de shaders para evitar 
                                    * recompilar o mesmo shader várias vezes.

        * Tratamento de Includes GLSL: Adicionar suporte para includes no código-fonte 
                                                * do shader, semelhante a arquivos .h em C.

*/

static GLuint make_shader(GLenum type, const char *filename)
{
    GLint length;
    GLchar *source = file_contents(filename, &length);
    GLuint shader;
    GLint shader_ok;

    if(!source)
    {
        return 0;
    }

    shader = glCreateShader(type);
    if (shader == 0)
    {
        fprintf(stderr, "Erro: Falha ao criar shader do tipo %d.\n", type);
        free(source);
        return 0;
    }
    glShaderSource(shader, 1, (const GLchar**)&source, &length);
    free(source);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
    if (!shader_ok) 
    {
        fprintf(stderr, "Failed to compile %s:\n", filename);
        show_info_log(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

/* Exemplo de Cache Simples (Manual) */
GLuint load_or_compile_shader(GLenum type, 
                              const char *shader_code, 
                              const char *cache_file) 
{
    FILE *file = fopen(cache_file, "rb");
    GLuint shader;
    long length;
    GLchar *binary;
    GLint success;

    if(file) 
    {
        // Se o cache existir, carregue e compile o shader
        shader = glCreateShader(type);
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);

        binary = malloc(length);
        fread(binary, 1, length, file);
        fclose(file);

        glShaderSource(shader, 1, (const GLchar **)&binary, NULL);
        free(binary);
    } 
    else 
    {
        // Compila o shader normalmente e salva o código
        shader = glCreateShader(type);
        glShaderSource(shader, 1, &shader_code, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success) 
        {
            // Salva o shader compilado em cache
            file = fopen(cache_file, "wb");
            fwrite(shader_code, 1, strlen(shader_code), file);
            fclose(file);
        } 
        else 
        {
            fprintf(stderr, "Erro ao compilar shader!\n");
        }
    }

    return shader;
}


/** * 
make_program() 
    
    * tem como objetivo criar, 
        * linkar e verificar um programa de shader (shader program) no OpenGL. 
    
    * Ela combina um vertex shader e um fragment shader em um único programa 
                                        * que será utilizado para renderização.


    * glCreateProgram()
        *Cria um objeto de programa de shader e 
            * retorna um identificador único (GLuint). 
        * Esse objeto é usado para combinar múltiplos shaders em um pipeline gráfico.

    * glAttachShader()
        * Anexa um vertex shader e um fragment shader ao programa. 
            *Isso diz ao OpenGL que esses shaders fazem parte do mesmo pipeline.

        * glAttachShader(program, vertex_shader) → Associa o vertex shader ao programa.
        * glAttachShader(program, fragment_shader) → Associa o fragment shader ao programa.

    * glLinkProgram()
        * Realiza o link dos shaders anexados no programa.
        * O processo de linkagem cria um pipeline de renderização que 
                                * pode ser usado para renderizar objetos.


    * glGetProgramiv(program, GL_LINK_STATUS, &program_ok)
        * Verifica se a linkagem foi bem-sucedida, 
            * armazenando o resultado em program_ok.

        * GL_LINK_STATUS retorna GL_TRUE se o link foi bem-sucedido, ou GL_FALSE se houve erro

    * Erro de Linkagem:
        * Caso o link falhe (program_ok == 0), a função:
            * Imprime uma mensagem de erro no console.
            * Chama a função show_info_log para exibir o log detalhado do erro.
            * Deleta o programa com glDeleteProgram(program).
            * Retorna 0 para indicar falha.

    * return program;
        * Se a linkagem for bem-sucedida, 
            * a função retorna o identificador do programa que 
                * pode ser usado com glUseProgram(program) para 
                    * ativar o programa durante a renderização.


** ================================= **
* Futuras Possibilidades de Expansão:
    *Adição de Geometry Shaders:
        *Caso você migre para uma versão posterior do OpenGL (3.2+), poderia adicionar um geometry shader ao pipeline.


    ``` glAttachShader(program, geometry_shader); ``` 
    * Validação do Programa:
        * Após a linkagem, 
            * você pode validar o programa para garantir que 
                * ele funcione corretamente no contexto atual do OpenGL.
            ``` glValidateProgram(program); ```


    * Uniform Blocks e Shader Storage:
        * Em versões mais recentes, 
            * você pode expandir a função para 
                * utilizar uniform blocks e shader storage buffers.

** ============ ** 
* Exemplo de Uso:
    ``` 
        GLuint vertex_shader = make_shader(GL_VERTEX_SHADER, "vertex_shader.glsl");
        GLuint fragment_shader = make_shader(GL_FRAGMENT_SHADER, "fragment_shader.glsl");

        if(vertex_shader && fragment_shader) 
        {
            GLuint shader_program = make_program(vertex_shader, fragment_shader);
            if (shader_program) 
            {
                glUseProgram(shader_program);  // Ativa o programa de shaders para renderização
            }
        }
    ``` 
** ======== **
* Conclusão:
    * A função make_program cria e configura o pipeline de renderização no OpenGL 2.0, 
        * combinando um vertex shader e um fragment shader. 
    * Embora seja simples, ela é uma peça fundamental para controlar como a GPU renderiza objetos.

*/
static GLuint make_program(GLuint vertex_shader, GLuint fragment_shader)
{
    GLint program_ok;

    GLuint program = glCreateProgram(); // Cria um objeto de programa de shader

    glAttachShader(program, vertex_shader);   // Anexa o vertex shader ao programa
    glAttachShader(program, fragment_shader); // Anexa o fragment shader ao programa
    glLinkProgram(program); // Faz o link dos shaders anexados no programa

    glGetProgramiv(program, GL_LINK_STATUS, &program_ok); // Verifica se o link foi bem-sucedido
    if (!program_ok) 
    {
        fprintf(stderr, "Failed to link shader program:\n");
        show_info_log(program, glGetProgramiv, glGetProgramInfoLog); // Exibe o log de erro
        glDeleteProgram(program); // Deleta o programa caso o link falhe
        return 0;
    }

    return program; // Retorna o identificador do programa vinculado com sucesso
}


/* atalho mais rapido para criação de shader */
void createShader(GLuint *programObject, char *vertexshaderfile, char *fragmentshaderfile) 
{
    GLuint vertexShader;
    GLuint fragmentShader;

    vertexShader = make_shader(GL_VERTEX_SHADER, vertexshaderfile);
    fragmentShader = make_shader(GL_FRAGMENT_SHADER, fragmentshaderfile);

    *programObject = make_program(vertexShader, fragmentShader);

}





#endif /* GL_COMMAND_UTILS_H */
