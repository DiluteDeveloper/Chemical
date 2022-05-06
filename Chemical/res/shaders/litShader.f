#version 460 core

out vec4 fragColor;

in vec2 f_texCoord;
in vec3 f_normal;
in vec3 f_fragPos;

//layout(binding = 0) uniform sampler2D f_texture;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

#define MAX_POINT_LIGHTS 10

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform Material material;
uniform vec3 viewPos;

vec3 calcPointLight(PointLight light, vec3 viewDir) {

    vec3 ambientResult = 0.1 * material.ambient * light.ambient; 
    vec3 diffuse = material.diffuse * light.diffuse;
    vec3 specular = 0.5 * material.specular * light.specular;

    vec3 lightDir = normalize(light.position - f_fragPos);

    float diffuseStrength = max(dot(f_normal, lightDir), 0.0);
    vec3 diffuseResult = diffuseStrength * diffuse;

    vec3 reflectDir = reflect(-lightDir, f_normal); // the reflected direction vector of where the light hits

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularResult = specular * spec;

    return vec3(diffuseResult + specularResult + ambientResult);
}

void main()
{
    vec3 viewDir = normalize(viewPos - f_fragPos);
    vec3 result;
    for(unsigned int i = 0; i < MAX_POINT_LIGHTS; i++)
        result = result + calcPointLight(pointLights[i], viewDir);

    fragColor = vec4(result, 1.0);


    // Normals debug
    //fragColor = vec4(f_normal.xyz, 1.0);
}