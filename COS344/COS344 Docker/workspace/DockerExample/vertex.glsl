#version 330 core
layout(location=0)in vec3 vertexPos;
layout(location=1)in vec3 vertexCol;
out vec3 fragmentCol;
void main(){
    gl_Position=vec4(vertexPos,1);
    fragmentCol=vertexCol;
}
