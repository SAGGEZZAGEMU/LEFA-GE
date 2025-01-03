#ifndef TEXTUREDATA_H
#define TEXTUREDATA_H

typedef struct
{
	GLsizei width;
	GLsizei height;
	GLubyte* buffer;
}TextureData;

TextureData TextureData_create(GLubyte *buffer, 
							   GLsizei width, 
							   GLsizei height) 
{
    TextureData data;
    data.buffer = buffer;
    data.width = width;
    data.height = height;
    return data;
}

GLsizei TextureData_getWidth(TextureData *texture) 
{
    if (texture == NULL) return 0; // Validação de segurança
    return texture->width;
}

GLsizei TextureData_getHeight(TextureData *texture) 
{
    if (texture == NULL) return 0; // Validação de segurança
    return texture->height;
}

GLubyte* TextureData_getBuffer(TextureData *texture) 
{
    if (texture == NULL) return NULL; // Validação de segurança
    return texture->buffer;
}




#endif /* TEXTUREDATA_H */
