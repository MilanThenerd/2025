#version 330 core

in vec3 fragmentCol;
in vec3 crntPos;

out vec4 color;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 glassColor;
uniform float glassAlpha;

uniform bool isBackSheet;
uniform bool isGlass;
uniform bool isLight;
uniform bool isBottom;

uniform vec3 Normal;

void main()
{
  vec3 normal = normalize(Normal);
  vec3 lightDirection = normalize(lightPos - crntPos);
  float diffuse = max(dot(normal, lightDirection) , 0.0f);
  color = vec4(fragmentCol * diffuse * mix(lightColor, glassColor, glassAlpha), 1.0f);

  if(isGlass)
  {
    color = vec4(glassColor, glassAlpha);
  }
  if(isLight)
  {
    color = vec4(lightColor , 1.0f);
  }
  if(isBottom)
  {
    color = vec4(0.4 , 0.4 , 0.4 , 1.0);
  }
}