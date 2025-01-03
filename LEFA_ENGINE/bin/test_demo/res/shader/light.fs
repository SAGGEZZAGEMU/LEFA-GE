#version 110

uniform vec4 lightColor;

void main()
{
    gl_FragColor = lightColor;
}

/*
#version 330 core

out vec4 FragColor;

uniform vec4 lightColor;

void main()
{
    FragColor = lightColor;
}*/