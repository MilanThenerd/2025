#version 330 core

in vec4 fragmentCol;
in vec3 Normal;
in vec3 FragPos;
out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform vec3 viewPos;

uniform float cutOff;  
uniform float outerCutOff;

void main() {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse 
    vec3 norm = normalize(vec3(0.0 , 1.0 ,0.0));
    vec3 lightDirActual = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDirActual), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirActual, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    // Spotlight (soft edges)
    float theta = dot(lightDirActual, normalize(-lightDir));
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
    
    // Combine results
    vec3 result = (ambient + (diffuse + specular) * intensity) * fragmentCol.rgb;
    fragColor = vec4(result, fragmentCol.a);
}