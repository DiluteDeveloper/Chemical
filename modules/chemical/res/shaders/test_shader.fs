#version 460 core

out vec4 fragColor;

in vec2 f_texCoords;

uniform vec3 colour;
layout(binding=0) uniform sampler2D albedo;
void main()
{                          
    fragColor = texture(albedo, f_texCoords) * vec4(colour,1.0);
}
