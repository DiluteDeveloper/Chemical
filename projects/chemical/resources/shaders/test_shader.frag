#version 460 core

layout(location=0) out vec4 fragColour;

in vec2 f_texCoord;

uniform sampler2D f_texture;

void main()
{   
    fragColour = texture(f_texture, f_texCoord);

}