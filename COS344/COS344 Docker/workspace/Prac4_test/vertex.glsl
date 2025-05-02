#version 330 core

layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 vertexCol;

out vec3 fragmentCol;
out vec3 crntPos;

uniform mat4 MVP;
uniform mat4 Model; 

void main()
{
    gl_Position = MVP * vec4(vertexPos, 1.0);
    fragmentCol = vertexCol;
    
    crntPos = vec3(Model * vec4(vertexPos, 1.0));
}