/*#version 110

attribute vec3 position; // Atributo de posição do vértice
attribute vec3 color;    // Atributo de cor
attribute vec2 texcoord; // Atributo de coordenadas de textura

varying vec3 Color;      // Variação para passar cor ao fragment shader
varying vec2 Texcoord;   // Variação para passar coordenadas de textura ao fragment shader

uniform mat4 model;      // Matriz de modelo
uniform mat4 view;       // Matriz de visão
uniform mat4 proj;       // Matriz de projeção

void main()
{
    Color = color; // Envia a cor para o fragment shader
    Texcoord = texcoord; // Envia as coordenadas de textura para o fragment shader
    gl_Position = proj * view * model * vec4(position, 1.0); // Calcula a posição do vértice
}*/

#version 110

attribute vec3 position;
attribute vec3 normal;
attribute vec3 color;

varying vec3 fragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    fragColor = color;
    gl_Position = proj * view * model * vec4(position, 1.0);
}