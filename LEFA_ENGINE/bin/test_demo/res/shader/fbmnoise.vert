#version 110

uniform float time;
attribute vec3 vertexPosition;
varying vec3 vTexCoord3D;

void main(void) {
    vTexCoord3D = vertexPosition * 2.0 + vec3(0.0, 0.0, -time);
    gl_Position = gl_ModelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}