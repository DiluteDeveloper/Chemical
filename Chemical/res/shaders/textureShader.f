#version 460 core

out vec4 fragColor;

in vec2 f_texCoord;
in vec3 f_normal;
in vec3 f_fragPos;

layout(binding = 0) uniform sampler2D f_texture;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

uniform vec3 f_lightCol;
uniform vec3 f_lightPos;
uniform vec3 f_viewPos;

void main()
{

    vec3 ambientResult = 0.1 * material.ambient; 
    vec3 diffuse = 1.0 * material.diffuse;
    vec3 specular = 0.5 * material.specular;


    vec3 norm = normalize(f_normal);
    vec3 lightDir = normalize(f_lightPos - f_fragPos);

    float diffuseStrength = max(dot(norm, lightDir), 0.0);
    vec3 diffuseResult = diffuseStrength * diffuse;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(f_viewPos - f_fragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // the reflected direction vector of where the light hits

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularResult = specular * spec;

    vec3 result = (diffuseResult + ambientResult + specularResult) * f_lightCol;
    fragColor = vec4(result, 1.0);


    // Normals debug
    //fragColor = vec4(f_normals.xyz, 1.0);
}