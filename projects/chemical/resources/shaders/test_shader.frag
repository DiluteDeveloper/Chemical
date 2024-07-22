#version 460 core

in vec3 f_normal;
in vec3 f_pos;

out vec4 fragColor;

vec3 lightPos = vec3(10, 75, -25);
vec3 lightColor = vec3(255 / 255.0, 196 / 255.0, 94 / 255.0);
vec3 objectColor = vec3(1,1,1);

void main()
{                          
    vec3 norm = normalize(f_normal);
    vec3 light_dir = normalize(lightPos - f_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * lightColor;

    fragColor = vec4(diffuse * objectColor, 1.0);
}