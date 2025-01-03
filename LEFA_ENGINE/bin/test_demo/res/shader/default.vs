/*#version 110

// Atributos
attribute vec3 aPos;    // Posição
attribute vec3 aColor;  // Cor
attribute vec2 aTex;    // UV
attribute vec3 aNormal; // Normal

// Variações para o Fragment Shader
varying vec3 color;
varying vec2 texCoord;
varying vec3 Normal;
varying vec3 crntPos;

// Uniforms para matrizes
uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 projection;

void main()
{
    // Calcula a posição atual
    crntPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * camMatrix * vec4(crntPos, 1.0);

    // Passa os valores para o Fragment Shader
    color = aColor;
    texCoord = aTex;
    Normal = aNormal;
}
*/

#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;
// Normals (not necessarily normalized)
layout (location = 3) in vec3 aNormal;


// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the current position for the Fragment Shader
out vec3 crntPos;

// Imports the camera matrix from the main function
uniform mat4 camMatrix;
// Imports the model matrix from the main function
uniform mat4 model;

uniform mat4 projection;

void main()
{
    // calculates current position
    crntPos = vec3(model * vec4(aPos, 1.0f));
    // Outputs the positions/coordinates of all vertices
    gl_Position = projection * camMatrix * vec4(crntPos, 1.0);

    // Assigns the colors from the Vertex Data to "color"
    color = aColor;
    // Assigns the texture coordinates from the Vertex Data to "texCoord"
    texCoord = aTex;
    // Assigns the normal from the Vertex Data to "Normal"
    Normal = aNormal;
}