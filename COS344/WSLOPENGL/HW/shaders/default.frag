#version 330 core

in vec4 fragmentCol;
in vec3 Normal;
in vec3 crntPos;

out vec4 color;

uniform vec4 lightColor;
uniform vec3 lightPos;

void main() 
{  
  vec3 normal = normalize(Normal);
  vec3 lightDirection = normalize(lightPos - crntPos);

  float diffuse = max(dot(normal, lightDirection), 0.3f);

  vec4 color = vec4(fragmentCol.rgb * lightColor.rgb * diffuse, fragmentCol.a);
}