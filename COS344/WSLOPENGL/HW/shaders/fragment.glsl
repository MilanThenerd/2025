#version 330 core

in vec4 fragmentCol;
in vec3 crntPos;
in vec3 fragNormal;
out vec4 fragColor;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform bool lightEnabled;
uniform vec3 lightDirection;

uniform vec3 camPos;

vec4 spotLight() {
    float outerCone = 0.90f;
    float innerCone = 0.95f;

    vec3 normal = normalize(fragNormal);
    vec3 lightVec = lightPos - crntPos;
    float dist = length(lightVec);
    vec3 lightDir = normalize(lightVec);

    // Distance attenuation (inverse square with a small constant to avoid division by zero)
    float attenuation = 1.0 / (1.0 + 0.1 * dist + 0.01 * dist * dist);

    // Diffuse lighting
    float diffuse = max(dot(normal, lightDir), 0.0);

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16) * specularStrength;

    // Spotlight cone intensity
    float angle = dot(normalize(lightDirection), -lightDir);
    float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0, 1.0);

    // Combine with distance attenuation
    vec3 lightEffect = (diffuse + spec) * intensity * attenuation * lightColor.rgb;

    // Ambient lighting (applied everywhere)
    float ambient = 0.2;
    vec3 base = fragmentCol.rgb * ambient;

    return vec4(base + lightEffect * fragmentCol.rgb, fragmentCol.a);
}

void main() 
{
    if (!lightEnabled) 
    {
      float ambient = 0.20f;
      vec3 base = fragmentCol.rgb;
      vec3 lit = base * ambient;
      fragColor = vec4(lit, fragmentCol.a);
      return ;
    }

    fragColor = spotLight();
}
