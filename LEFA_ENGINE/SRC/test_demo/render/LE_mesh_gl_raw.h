#ifndef LE_MESH_GL_RAW
#define LE_MESH_GL_RAW

#include"LE_mesh.h"

typedef struct 
{
    GLenum type;              // Tipo do atributo (GL_FLOAT, GL_INT, etc.)
    GLint size;               // Número de componentes por atributo (ex: 3 para posição x, y, z)
    GLboolean normalized;     // Se os dados são normalizados (ex: cor entre 0 e 1)
    GLsizei stride;           // Distância entre o início de cada conjunto de dados de vértices
    const void *pointer;      // Deslocamento no buffer para começar a ler esse atributo
} VertexAttribute;

#define MAX_ATTRIB_NAME_LENGTH 128

typedef struct 
{
    GLenum  format;             // O formato de dado (ex: GL_FLOAT, GL_INT)
    GLint   size;               // O número de componentes por vértice (ex: 3 para um vetor 3D)
    GLuint  attribIndex;        // Índice do atributo, para glVertexAttribPointer
    GLint   offset;             // Deslocamento em bytes
} LayoutAttribDesc;

void setupVertexAttributes(GLuint vbo, VertexAttribute* layout, GLuint numAttributes) 
{
	GLuint i;
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // Vincula o VBO onde os dados de vértices estão armazenados
    for(i = 0; i < numAttributes; i++) 
    {
        // Configura o atributo de vértice
        glVertexAttribPointer(i, 
        					  layout[i].size, 
        					  layout[i].type, 
        					  layout[i].normalized,
                              layout[i].stride, 
                              layout[i].pointer);
        // Habilita o atributo de vértice
        glEnableVertexAttribArray(i);
    }
}

void drawObject(GLuint vbo, GLuint numVertices, VertexAttribute* layout, GLuint numAttributes) 
{
    // Configuração do layout dos vértices (atributos do vértice)
    setupVertexAttributes(vbo, layout, numAttributes);
    
    // Desenha o objeto (supondo um tipo de renderização TRIANGLES, por exemplo)
    glDrawArrays(GL_TRIANGLES, 0, numVertices);  // Renderiza o objeto com o número de vértices desejados
}

typedef struct {
    GLuint                     pVertexResource;       // Buffer para os dados de vértices (pode ser um VBO)
    GLuint                     pOffsetsResource;      // Buffer para offsets dos Blendshapes
    GLuint                     pWeightsResource;      // Buffer para pesos
    GLuint                     pVertexTexture;        // Textura que contém os dados dos vértices, se necessário
    GLuint                     pOffsetsTexture;       // Textura para offsets (usar em shaders se necessário)
    GLuint                     pWeightsTexture;       // Textura para pesos
    int                        numUsedBS;             // Quantidade de Blendshapes usadas
} BlendShapesResource;

typedef struct
{
  	FileHeader 			*pMesh;

    GLuint             	vbo;            // Handle para o Vertex Buffer Object
    GLuint             	ibo;            // Handle para o Index Buffer Object
    GLuint             	vao;            // Vertex Array Object (caso use OpenGL 3+ futuramente)

    int                numVertices;    // Número de vértices
    int                numIndices;     // Número de índices (se houver)
    int                numLayouts;     // Número de atributos de layout (como tipo, posição, cor, etc.)
    
    // Layout dos atributos de vértice
    VertexAttribute   *layout;         // Array de layouts dos atributos (posição, cor, etc.)
    GLuint             *vertexBuffers;   // Pontos para cada VBO de vértices
    GLuint             *indexBuffers;    // Pontos para os IBO de índices
  
  	BlendShapesResource       bsResource;

    GLuint                     *pVertexBuffers;     // Array de ponteiros para VBOs
    GLuint                     *pIndexBuffers;      // Array de ponteiros para buffers de índice
    //GLuint                     *pStreamOutBuffers;  // Array de ponteiros para buffers de stream out (não disponível no OpenGL 2.0)
    GLuint                     *pStrides;           // Strides de vértices, em caso de buffers intercalados
    GLint                      numVertexBuffers;    // Número de buffers de vértices
    GLint                      numIndexBuffers;     // Número de buffers de índice
    //GLint                      numStreamOutBuffers; // Número de buffers de stream out (não disponível diretamente no OpenGL 2.0)
    GLuint                     pLayout;             // Usaremos pLayout para o formato de entrada dos vértices, sem a necessidade de InputLayout no OpenGL 2.0

  //int                       numLayoutAttribs;
  //int                       numLayoutSlots;
  //int                       numVertexBuffers;
  //int                       numStreamOutBuffers;
  //int                       numIndexBuffers;

}LE_RAW_MESH_GL;

void InitializeRawMesh(LE_RAW_MESH_GL *mesh)
{
    // Inicializando variáveis a valores padrão
    //mesh->numLayoutSlots = 0;
    //mesh->numLayoutAttribs = 0;
    //mesh->numStreamOutBuffers = 0;
    //mesh->pLayoutDesc = NULL;         // Não usaremos Input Layouts diretamente
    mesh->pMesh = NULL;               // Supondo que `pMesh` seja um arquivo binário ou um ponteiro de estrutura (dependendo de como você lida com meshes)
    mesh->pVertexBuffers = NULL;      // Não alocamos memória até precisar
    mesh->pIndexBuffers = NULL;       // Do mesmo modo, para buffers de índices
    mesh->pLayout = 0;                // OpenGL não precisa de InputLayout diretamente, então podemos definir para 0
    mesh->numIndexBuffers = 0;
    mesh->numVertexBuffers = 0;
    mesh->pStrides = NULL;            // O valor inicial para strides não é necessário até que usemos
    //mesh->pStreamOutBuffers = NULL;   // StreamOut não é suportado no OpenGL 2.0, mantemos nulo

    // Você pode precisar alocar espaço para os buffers dinamicamente quando for utilizar
    // Por exemplo, algo como:
    // mesh->pVertexBuffers = malloc(numBuffers * sizeof(GLuint)); para os buffers de vértices

    // Ou dependendo do fluxo de trabalho, você pode usar funções de configuração como glGenBuffers diretamente na inicialização

    // Se você precisar criar buffers de vértices ou de índice, faça isso após essa função
    if (mesh->numVertexBuffers > 0)
    {
        mesh->pVertexBuffers = malloc(mesh->numVertexBuffers * sizeof(GLuint));  // Aloca espaço para os VBOs
        glGenBuffers(mesh->numVertexBuffers, mesh->pVertexBuffers);             // Gera os buffers de vértices
    }

    if (mesh->numIndexBuffers > 0)
    {
        mesh->pIndexBuffers = malloc(mesh->numIndexBuffers * sizeof(GLuint));  // Aloca espaço para os buffers de índice
        glGenBuffers(mesh->numIndexBuffers, mesh->pIndexBuffers);              // Gera os buffers de índice
    }

    // Da mesma forma, se algum outro tipo de buffer for necessário, aloque e gere como exemplificado

}

FileHeader loadRawMesh(LE_RAW_MESH_GL *mesh, LPCSTR fname)
{
	mesh->pMesh = loadMesh(fname);
    return *mesh->pMesh;
}


void destroy(LE_RAW_MESH_GL *mesh)
{
    int i;


    // Liberação de buffer de layout em OpenGL
    if(mesh->pLayout) 
    {
        // Não há equivalente exato no OpenGL, mas podemos garantir que o layout não está mais sendo usado
        glDeleteProgram(mesh->pLayout);  // Caso pLayout seja um programa
        mesh->pLayout = 0;
    }



    if(mesh->bsResource.pVertexResource) 
    {
        glDeleteBuffers(1, &mesh->bsResource.pVertexResource); // Para buffers de vértices
        mesh->bsResource.pVertexResource = 0;
    }


    if(mesh->bsResource.pOffsetsResource) 
    {
        glDeleteBuffers(1, &mesh->bsResource.pOffsetsResource);
        mesh->bsResource.pOffsetsResource = 0;
    }


    if(mesh->bsResource.pWeightsResource) 
    {
        glDeleteBuffers(1, &mesh->bsResource.pWeightsResource);
        mesh->bsResource.pWeightsResource = 0;
    }

    mesh->bsResource.numUsedBS = 0;

    // Liberação de buffers de vértices (OpenGL 2.0 utiliza buffers de vértices e não possui streaming específico como o D3D)
    if(mesh->pVertexBuffers) 
    {
        for(i = 0; i < mesh->numVertexBuffers; i++)
        {
            if(mesh->pVertexBuffers[i])
            {
                glDeleteBuffers(1, &mesh->pVertexBuffers[i]);
            }
        }
        free(mesh->pVertexBuffers); // Liberando o array de buffers
        mesh->pVertexBuffers = NULL;
    }

    // Liberação de buffers de índice
    if(mesh->pIndexBuffers) 
    {
        for(i = 0; i < mesh->numIndexBuffers; i++)
        {
            if(mesh->pIndexBuffers[i]) 
            {
                glDeleteBuffers(1, &mesh->pIndexBuffers[i]);
            }
        }
        free(mesh->pIndexBuffers);  // Liberando o array de buffers de índice
        mesh->pIndexBuffers = NULL;
    }

    // Liberação de strides (se houver buffers associados a elas)
    if(mesh->pStrides)
    {
        free(mesh->pStrides);  // OpenGL não precisa de "strides" manualmente, a não ser que esteja usando algum tipo de formato personalizado.
        mesh->pStrides = NULL;
    }




    if(mesh->pMesh) 
    {
        free(mesh->pMesh);  // Liberar memória associada à malha
        mesh->pMesh = NULL;
    }
}

void releaseRawBufferData(LE_RAW_MESH_GL *mesh, FileHeader *fileheader)
{
    if(mesh->pMesh)
    { 
        mesh->pMesh = releaseBufferData(fileheader);
    }
}

/*
void generateLayoutDesc(LE_RAW_MESH_GL *mesh, int appendBlendshapes, int slotstart)
{
    int nbsattr = mesh->pMesh->bsLayout.num_attribs;
    int totalAttribs = mesh->pMesh->layout.num_attribs + (appendBlendshapes * nbsattr);
    LayoutAttribDesc* pLayoutDesc;
    int layoutnum = 0;
    int i, j;
    int slotnum;





    // Alocar memória para a descrição dos atributos
    pLayoutDesc = (LayoutAttribDesc*)malloc(sizeof(LayoutAttribDesc) * totalAttribs);
    if (!pLayoutDesc) 
    {
        fprintf(stderr, "Erro: Falha ao alocar memória para LayoutDesc\n");
        return;
    }

    for(i = 0; i < mesh->pMesh->layout.num_attribs; i++) 
    {
        pLayoutDesc[layoutnum].format = mesh->pMesh->layout.attribs[i].formatGL;
        pLayoutDesc[layoutnum].size = mesh->pMesh->layout.attribs[i].size;
        pLayoutDesc[layoutnum].attribIndex = mesh->pMesh->layout.attribs[i].index;
        pLayoutDesc[layoutnum].offset = mesh->pMesh->layout.attribs[i].AlignedByteOffset;
        layoutnum++;
    }

    // Manipulação dos Blendshapes, caso seja solicitado
    if (appendBlendshapes > 0) 
    {
        slotnum = (slotstart == 0) ? mesh->pMesh->numSlots : slotstart;
        for(i = 0; i < appendBlendshapes; i++) 
        {
            for (j = 0; j < nbsattr; j++) 
            {
                char name[MAX_ATTRIB_NAME_LENGTH];
                snprintf(name, sizeof(name), "bs_%s", mesh->pMesh->layout.attribs[j].name);

                pLayoutDesc[layoutnum].format = mesh->pMesh->bsLayout.attribs[j].formatGL;
                pLayoutDesc[layoutnum].size = mesh->pMesh->bsLayout.attribs[j].size;
                pLayoutDesc[layoutnum].attribIndex = slotnum;
                pLayoutDesc[layoutnum].offset = mesh->pMesh->bsLayout.attribs[j].AlignedByteOffset;
                layoutnum++;
            }
            slotnum++;
        }
    }


    // Agora, use o OpenGL para configurar esses atributos de vértice
    for(i = 0; i < totalAttribs; i++) 
    {
        glEnableVertexAttribArray(pLayoutDesc[i].attribIndex);
        glVertexAttribPointer(pLayoutDesc[i].attribIndex, 
                              pLayoutDesc[i].size, 
                              pLayoutDesc[i].format, 
                              GL_FALSE, 
                              sizeof(float) * pLayoutDesc[i].size, 
                              (const void*)pLayoutDesc[i].offset);
    }

    // O layout do buffer foi configurado
    free(pLayoutDesc);
}
*/


#endif /* LE_MESH_GL_RAW */
