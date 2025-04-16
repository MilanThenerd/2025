#version 330 core
layout(location=0)in vec3 vertexPos;
layout(location=1)in vec4 vertexCol;
layout(location=2)in vec3 aNormal;

out vec4 fragmentCol;
out vec3 crntPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
  crntPos = vec3(model * vec4(vertexPos, 1.0));
  gl_Position = projection * view  * vec4(vertexPos, 1.0);
  fragmentCol= vertexCol;
  Normal = mat3(transpose(inverse(model))) * aNormal; 
}
