#version 460 core

out vec4 fragColor;

in vec3 f_normal;
in vec3 f_fragPos;
uniform vec3 viewPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    uint shininess;
}; 
uniform Material material;

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

vec3 calcPointLight(vec3 viewDir) {

		PointLight light;
		light.position = vec3(2,2,-10);
		light.diffuse = vec3(0.7, 0.7, 0.7);
		light.specular= vec3(1.0, 1.0, 1.0);
		light.ambient = vec3(0.2, 0.2, 0.2);
		
    vec3 ambientResult = material.ambient * light.ambient; 
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
    vec3 result = calcPointLight(viewDir);

    fragColor = vec4(result, 1.0);
		//fragColor = vec4(f_normal, 1.0);


    // Normals debug
    //fragColor = vec4(f_normal.xyz, 1.0);
}
