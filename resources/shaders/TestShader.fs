#version 460 core

layout(location=0) out vec4 fragColour;

in vec3 f_colour;
in vec2 f_texCoord;

uniform sampler2DArray f_texture;

void main()
{   
    vec3 tCoord = vec3(f_texCoord.xy, 0);
    fragColour = texture(f_texture, tCoord);

}