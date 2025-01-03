#ifndef LE_MESH_H
#define LE_MESH_H

#include"test_demo/localmath.h"
#include"3rdparty/glad/OpenGL/glad_gl.h"

#ifndef NOGZLIB
	#include"3rdparty/zlib/zlib.h"
	#define GFILE gzFile
	#define GOPEN gzopen
	#define GREAD(a,b,c) gzread(a, b, c)
	#define GCLOSE gzclose
#else
	#define GFILE FILE *
	#define GOPEN fopen
	#define GREAD(a,b,c) (int)fread(b, 1, c, a)
	#define GCLOSE fclose
#endif

#ifndef LOGMSG
  #	define LOG_MSG stdout
  #	define LOG_WARN stdout
  #	define LOG_YES stdout
  #	define LOG_NO stdout
  #	define LOG_NOTE stdout
  #	define LOG_ERR stderr
  #define LOGMSG fprintf
#endif
#ifndef CONSOLEMSG
  # define CONSOLEMSG LOGMSG
#endif
  
  #define RAWMESHVERSION 0x105



typedef GLenum GLTopology;
typedef GLenum GLType;

typedef struct
{
	int u;

}LE_Mesh;

typedef struct
{
	GLType              formatGL;               // doesn't say how many components. See numComp.
    unsigned char       semanticIdx;
    unsigned char       numComp;
	//TODO: no need for this slot info since Attribute will belong to the slot
    unsigned char       slot;
    unsigned int        strideBytes;            // strideBytes info for OpenGL
    unsigned int        AlignedByteOffset;      // DirectX10 needs it in the Layout definition : offset in the layout.
    unsigned int        dataOffsetBytes;        // for example, use this for VertexPointer() in VBO or what we do with offsetof(VertexStructure, p)...
    void                *pAttributeBufferData;  // this pointer will have to be resolved after load operation
    unsigned int        nameOffset;             // must be pointing to namebuffer
    char                name[16];               // could be somewhere else
}Attribute;

//--------------------------------
// 
//--------------------------------
// TODO : remove it since we may want to put the info in Slot
typedef struct
{
    int           num_attribs;
    Attribute     attribs[16]; // Max = 16 = D3D10_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT
}Layout;

//--------------------------------
// 
// a slot/stream is made of interleaved attribs. In the easiest case it has only one attr.
// Note that this slotDataOffsets/pVtxBufferData may contain a subset of vertices instead of the whole : if the slot
// is being used on a smaller part of the mesh, no need to provide all.
// TODO : indices to shift.
//--------------------------------
typedef struct 
{
    unsigned int        vtxBufferSizeBytes;     // size of the vertex buffer, in bytes
    unsigned int        vtxBufferStrideBytes;   // strideBytes of the vertex buffer
    unsigned int        vertexCount;            // [DX9 wants it] Num used vertices
    unsigned int        slotDataOffsets;
    void                *pVtxBufferData;     // this pointer will have to be resolved after load operation
    char                name[32];               // for example, use in blendshapes : a slot is a blendshape. Good to have its name
    // TODO : layout infos
    // unsigned int     numAttribs;
    // Attribute        attribs[16];
}Slot;

/*--------------------------------
	Primitive group
	TO CHECK:
	* can be created after stripifier process : 
	one triangle list primitive group is split to many tristrip primitive group
	NOT sure : seems it will always endup with 1 single tristrip (+degenerated tris)
	QUESTION ? Do we want to add primitive restart
	* one primitive group for one 'shader' : vertices associated with a shader will be referenced by a primitive group
	QUESTION : one primgroup will use a specific set of streams. Give them.
	simplest case : 1 stream for 1 primgroup. However we can have many streams for 1 primgroup
	* a primitive group can work only with a smaller set of vertices.
--------------------------------*/
typedef struct
{
  	/*
    void init()
    {
    	memset((void*)this, 0, sizeof(PrimGroup));
    }*/

	void(*PrimGroup_Init)(void *self);

   	char name[64];
    //TODO : unsigned int       numSlots;
    // use an array of offsets + ptr to resolve, instead ?
    //TODO : unsigned int       slotReferences[16]; ///< references to used slots
    unsigned int                indexCount;             // total number of elements
    unsigned int                minIndex;               // min element index
    unsigned int                maxIndex;               // max element index
    unsigned int                indexArrayByteOffset;   // offset to reach the element array from &data[1]
    unsigned int                indexArrayByteSize;     // total size in bytes
    unsigned int                primitiveCount;         // DX9 wants it

    GLType                      indexFormatGL;

    GLTopology                  topologyGL;
    void                        *pIndexBufferData;       // this pointer will have to be resolved after load operation
}PrimGroup;


typedef struct 
{
    unsigned int magic;
    unsigned int version;
    unsigned int size;
    char meshName[64];
    Layout layout;
    unsigned int numSlots;
    Slot slots[16];
    Layout bsLayout;
    unsigned int numBlendShapes;
    unsigned int blendShapesOffset;
    Slot *bsSlots;
    int numPrimGroups;
    PrimGroup primGroup[1];
    char rawdata[1];

    void (*resolvePointers)(void);
}FileHeader;


void PrimGroup_Init(PrimGroup *self)
{ 
	memset((void*)self, 0, sizeof(PrimGroup));
}

void FileHeader_init(FileHeader *header) 
{
    strncpy((char *)&header->magic, "MESH", 4);
    header->version = 0x01; /* Definir versão apropriada */
    header->size = sizeof(FileHeader);
    header->numPrimGroups = 0;
    header->numSlots = 0;
    header->numBlendShapes = 0;
    header->blendShapesOffset = 0;
    header->bsSlots = NULL;
    memset(&(header->primGroup[0]), 0, sizeof(PrimGroup));
}

void FileHeader_resolvePointers(FileHeader *header) 
{
    int i;
    header->bsSlots = (Slot *)(header->rawdata + header->blendShapesOffset);
    for (i = 0; i < header->numPrimGroups; i++) 
    {
        header->primGroup[i].pIndexBufferData = header->rawdata + header->primGroup[i].indexArrayByteOffset;
    }
    for (i = 0; i < (int)header->numSlots; i++) 
    {
        header->slots[i].pVtxBufferData = header->rawdata + header->slots[i].slotDataOffsets;
    }
    for (i = 0; i < (int)header->numBlendShapes; i++) 
    {
        header->bsSlots[i].pVtxBufferData = header->rawdata + header->bsSlots[i].slotDataOffsets;
    }
    for (i = 0; i < header->layout.num_attribs; i++) 
    {
        header->layout.attribs[i].pAttributeBufferData = header->rawdata + header->layout.attribs[i].dataOffsetBytes;
    }
}


void FileHeader_debugDumpLayout(const FileHeader *header)
{
    int i;
    printf("\nMesh Infos: %s\nAttributes:\n", header->meshName);
    for (i = 0; i < header->layout.num_attribs; i++)
    {
        printf("Name: %s\n", header->layout.attribs[i].name);
        printf("  Slot: %d\n", header->layout.attribs[i].slot);
        printf("  Number of Components: %d\n", header->layout.attribs[i].numComp);
        printf("  Stride (bytes): %d\n", header->layout.attribs[i].strideBytes);
        printf("  Format GL: %d\n", header->layout.attribs[i].formatGL);
    }

    printf("\nPrimitive groups: %d\n", header->numPrimGroups);
    for (i = 0; i < header->numPrimGroups; i++)
    {
        printf("\nPrim group %d : %s\n", i, header->primGroup[i].name);
        printf("  Number of elements: %d\n", header->primGroup[i].indexCount);
        printf("  Min element: %d\n", header->primGroup[i].minIndex);
        printf("  Max element: %d\n", header->primGroup[i].maxIndex);
        printf("  Primitive Count: %d\n", header->primGroup[i].primitiveCount);
        printf("  Topology GL: %d\n", header->primGroup[i].topologyGL);
    }
}

static FileHeader* loadMesh(const char* fname) 
{
	char* memory;
    GFILE* fd = NULL;
    int offs = 0;
    int n = 0;
    FileHeader* header;


    fd = GOPEN(fname, "rb");
    if (!fd) 
    {
        LOGMSG(stderr, "Error: Couldn't load %s\n", fname);
        return NULL;
    }

    #define RAWMESHMINSZ (1024*1000)
    memory = (char*)malloc(RAWMESHMINSZ);
    if (!memory) 
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        GCLOSE(fd);
        return NULL;
    }

    do 
    {
        if (n > 0) 
        {
            offs += RAWMESHMINSZ;
            memory = (char*)realloc(memory, RAWMESHMINSZ + offs);
            if (!memory) 
            {
                fprintf(stderr, "Error: Memory reallocation failed\n");
                GCLOSE(fd);
                return NULL;
            }
        }
        n = GREAD(fd, memory + offs, RAWMESHMINSZ);
    } 
    while(n == RAWMESHMINSZ);

    if (n > 0) 
    {
        offs -= RAWMESHMINSZ - n;
        memory = (char*)realloc(memory, RAWMESHMINSZ + offs);
        if (!memory) 
        {
            fprintf(stderr, "Error: Memory reallocation failed\n");
            GCLOSE(fd);
            return NULL;
        }
    }

    GCLOSE(fd);

    if (strncmp(memory, "MESH", 4) != 0) 
    {
        fprintf(stderr, "Error: Not a mesh file\n");
        free(memory);
        return NULL;
    }

    header = (FileHeader*)memory;
    if (header->version != RAWMESHVERSION) 
    {
        fprintf(stderr, "Error: Wrong version in Mesh description\n");
        free(memory);
        return NULL;
    }

    if (header->resolvePointers) 
    {
        header->resolvePointers();
    }

    return header;
}

static FileHeader* releaseBufferData(FileHeader* p) 
{
    int i;

    if (p == NULL) 
    {
        return NULL; /* Evita desreferenciar ponteiro nulo */
    }

    /* Limpa os dados do buffer de índices nos grupos primitivos */
    for (i = 0; i < p->numPrimGroups; i++) {
        p->primGroup[i].pIndexBufferData = NULL;
    }

    /* Limpa os dados do buffer de vértices nos slots */
    for (i = 0; i < p->numSlots; i++) 
    {
        p->slots[i].pVtxBufferData = NULL;
    }

    /* Limpa os atributos no layout */
    for (i = 0; i < p->layout.num_attribs; i++) 
    {
        p->layout.attribs[i].pAttributeBufferData = NULL;
    }

    /* Realoca a estrutura do cabeçalho */
    return (FileHeader*)realloc(p, sizeof(FileHeader));
}




#endif /* LE_MESH_H */
