#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 MVP;
uniform mat4 model;

out vec4 fragmentCol;
out vec3 FragPos;

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    fragmentCol = color;
    FragPos = vec3(model * vec4(position, 1.0));
}