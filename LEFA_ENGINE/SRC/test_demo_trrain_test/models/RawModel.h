#ifndef RAWMODEL_H
#define RAWMODEL_H

typedef struct
{
	GLuint vaoID;
	int vertexCount;

	int indexCount; /* PICANHA ADICIONAL */

}RawModel ;



void RawModel_init(RawModel* model,  
				   GLuint vaoID, 
				   int vertexCount) 
{
    if (model == NULL) return; // Evitar ponteiro nulo
    model->vaoID = vaoID;
    model->vertexCount = vertexCount;
}

GLuint RawModel_getVaoID(const RawModel *model) 
{
    return model->vaoID;
}

int RawModel_getVertexCount(const RawModel *model) 
{
    return model->vertexCount;
}



#endif

