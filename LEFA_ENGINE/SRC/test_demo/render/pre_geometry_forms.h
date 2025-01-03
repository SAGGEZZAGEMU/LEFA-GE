#ifndef PRE_GEOMETRY_FORMS_H
#define PRE_GEOMETRY_FORMS_H



// Função para desenhar um cubo
static 
void drawCube(float x, float y, float z) 
{
    // Vértices para o cubo
    GLfloat vertices[] = 
    {
        // Frente
        -0.5f, -0.5f,  0.5f,  // Inferior esquerdo
        0.5f, -0.5f,  0.5f,  // Inferior direito
        0.5f,  0.5f,  0.5f,  // Superior direito
        -0.5f,  0.5f,  0.5f,  // Superior esquerdo
        // Trás
        -0.5f, -0.5f, -0.5f,  // Inferior esquerdo
        0.5f, -0.5f, -0.5f,  // Inferior direito
        0.5f,  0.5f, -0.5f,  // Superior direito
        -0.5f,  0.5f, -0.5f   // Superior esquerdo
    };

    // Índices para formar os triângulos do cubo
    GLuint indices[] = 
    {
        0, 1, 2, 2, 3, 0,       // Frente
        4, 5, 6, 6, 7, 4,       // Trás
        0, 4, 7, 7, 3, 0,       // Esquerda
        1, 5, 6, 6, 2, 1,       // Direita
        3, 7, 6, 6, 2, 3,       // Topo
        0, 4, 5, 5, 1, 0        // Fundo
    };

    glPushMatrix();
    glTranslatef(x, y, z);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);

    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}


static 
void drawTexturedSphere(float radius, int segments, float x, float y, float z) 
{
    int i, j, vertIndex = 0, index = 0;
    int totalVertices = (segments + 1) * (segments + 1);
    int totalIndices = segments * segments * 6;

    // Alocações dinâmicas para vértices e índices
    float *vertices = (float *)malloc(totalVertices * 3 * sizeof(float));
    float *normals = (float *)malloc(totalVertices * 3 * sizeof(float));
    float *texCoords = (float *)malloc(totalVertices * 2 * sizeof(float));
    unsigned int *indices = (unsigned int *)malloc(totalIndices * sizeof(unsigned int));

    // Geração de vértices, normais e coordenadas de textura
    for (j = 0; j <= segments; j++) 
    {
        float theta = j * M_PI / segments; // Latitude
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (i = 0; i <= segments; i++) 
        {
            float phi = i * 2.0f * M_PI / segments; // Longitude
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float xPos = radius * sinTheta * cosPhi;
            float yPos = radius * sinTheta * sinPhi;
            float zPos = radius * cosTheta;

            // Vértices
            vertices[vertIndex * 3] = xPos;
            vertices[vertIndex * 3 + 1] = yPos;
            vertices[vertIndex * 3 + 2] = zPos;

            // Normais
            normals[vertIndex * 3] = sinTheta * cosPhi;
            normals[vertIndex * 3 + 1] = sinTheta * sinPhi;
            normals[vertIndex * 3 + 2] = cosTheta;

            // Coordenadas de textura UV
            texCoords[vertIndex * 2] = (float)i / segments;
            texCoords[vertIndex * 2 + 1] = 1.0f - (float)j / segments;

            vertIndex++;
        }
    }

    // Geração de índices para os triângulos
    for(j = 0; j < segments; j++) 
    {
        for (i = 0; i < segments; i++) 
        {
            int first = j * (segments + 1) + i;
            int second = first + segments + 1;

            // Triângulo 1
            indices[index++] = first;
            indices[index++] = second;
            indices[index++] = first + 1;

            // Triângulo 2
            indices[index++] = second;
            indices[index++] = second + 1;
            indices[index++] = first + 1;
        }
    }

    // Desenho com VBO
    glPushMatrix();
    glTranslatef(x, y, z);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glNormalPointer(GL_FLOAT, 0, normals);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

    glDrawElements(GL_TRIANGLES, totalIndices, GL_UNSIGNED_INT, indices);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();

    // Liberação de memória
    free(vertices);
    free(normals);
    free(texCoords);
    free(indices);
}



static 
void drawPlane(float x, float y, float z, float scale, int orientation)
{
    glPushMatrix();

    // Posiciona o plano em (x, y, z)
    glTranslatef(x, y, z);

    // Configura a orientação: 
    // 0 - Deitado (XY), 1 - Em pé no XZ, 2 - Em pé no YZ
    switch (orientation)
    {
        case 0: // Plano XY (Deitado) - padrão
            break;
        case 1: // Plano XZ (em pé vertical no eixo Z)
            glRotatef(90, 1, 0, 0);
            break;
        case 2: // Plano YZ (em pé vertical no eixo Y)
            glRotatef(90, 0, 1, 0);
            break;
        default:
            break; // Padrão: Sem alteração
    }

    // Escala o plano
    glScalef(scale, scale, scale);

    // Renderiza o plano como um TRIANGLE_STRIP
    glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f); // Vértice inferior esquerdo
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);  // Vértice inferior direito
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 0.0f);  // Vértice superior esquerdo
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);   // Vértice superior direito
    glEnd();

    glPopMatrix();
}


// Renderiza a pirâmide com controle de posição, tamanho e ângulo
static 
void drawPyramid(float posX, float posY, float posZ, float scale, float rotationAngle) /* FIXME */
{
    int i, j;

    // Vertices da pirâmide
    GLfloat vertices[][3] = {
        {-0.5f, 0.0f,  0.5f}, // 0: Frente esquerda
        {-0.5f, 0.0f, -0.5f}, // 1: Traseira esquerda
        { 0.5f, 0.0f, -0.5f}, // 2: Traseira direita
        { 0.5f, 0.0f,  0.5f}, // 3: Frente direita
        { 0.0f, 0.8f,  0.0f}  // 4: Topo
    };

    // Cores para os vértices
    GLfloat colors[][3] = {
        {0.83f, 0.70f, 0.44f}, // Cor base
        {0.83f, 0.70f, 0.44f},
        {0.83f, 0.70f, 0.44f},
        {0.83f, 0.70f, 0.44f},
        {0.92f, 0.86f, 0.76f}  // Cor do topo
    };

    // Índices para formar os triângulos
    GLuint indices[][3] = {
        {0, 1, 2}, // Base 1
        {0, 2, 3}, // Base 2
        {0, 4, 1}, // Lado esquerdo
        {1, 4, 2}, // Lado traseiro
        {2, 4, 3}, // Lado direito
        {3, 4, 0}  // Lado frente
    };

    // Transforma a matriz para aplicar posição, escala e rotação
    glPushMatrix();

    glTranslatef(posX, posY, posZ); // Translada a pirâmide para a posição desejada
    glScalef(scale, scale, scale); // Aplica a escala
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); // Rotação em torno do eixo Y

    glBegin(GL_TRIANGLES);
    for(i = 0; i < 6; i++)  // 6 faces no total
    {
        for (j = 0; j < 3; j++) 
        {
            int vertexIndex = indices[i][j];
            glColor3fv(colors[vertexIndex]); // Define a cor
            glVertex3fv(vertices[vertexIndex]); // Define o vértice
        }
    }
    glEnd();

    glPopMatrix(); // Restaura a matriz original
}
















#endif /* PRE_GEOMETRY_FORMS_H */
