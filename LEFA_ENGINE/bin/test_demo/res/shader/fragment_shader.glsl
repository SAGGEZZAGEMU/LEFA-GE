#version 110

varying vec3 Color;      // Recebe a cor do vertex shader
varying vec2 Texcoord;   // Recebe as coordenadas de textura do vertex shader

uniform sampler2D texKitten; // Textura do "gatinho"
uniform sampler2D texPuppy;  // Textura do "cachorrinho"

void main()
{
    // Realiza a mistura das texturas com pesos iguais (50%)
    gl_FragColor = mix(texture2D(texKitten, Texcoord), texture2D(texPuppy, Texcoord), 0.5);
}