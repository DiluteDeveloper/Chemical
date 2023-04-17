#version 460 core

out vec4 fragColor;

in vec2 f_texCoord;

in vec3 f_normal;
in vec3 f_pos;

uniform sampler2D f_texture;
uniform vec3 f_diffuse;
uniform vec3 f_lightPos;
uniform vec3 f_viewPos;

void main()
{
    float ambientStrength = 0.1;

    // diffuse 
    vec3 norm = normalize(f_normal);
    vec3 lightDir = normalize(f_lightPos - f_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(f_viewPos - f_pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float specular = specularStrength * spec;  
        
    vec3 result = (vec3(ambientStrength) + vec3(diff*1.2) + vec3(specular)) * f_diffuse;
    fragColor = vec4(result, 1.0) * texture(f_texture, f_texCoord);
}