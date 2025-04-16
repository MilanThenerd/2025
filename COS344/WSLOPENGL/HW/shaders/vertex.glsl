#version 330 core
layout(location=0) in vec3 vertexPos;
layout(location=1) in vec4 vertexCol;
layout(location=2) in vec3 vertexNormal; 

out vec4 fragmentCol;
out vec3 crntPos;
out vec3 normal;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(vertexPos,1.0);
    fragmentCol = vertexCol;
    crntPos = vertexPos;
    normal = vertexNormal;
}
