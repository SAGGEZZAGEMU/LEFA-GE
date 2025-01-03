#version 110

uniform float time;

varying vec3 vTexCoord3D;

void main() {
    vTexCoord3D = gl_Vertex.xyz * 4.0 - vec3(0.0, 0.0, time);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}