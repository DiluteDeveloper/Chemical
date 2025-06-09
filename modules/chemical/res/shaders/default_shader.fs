#version 460 core

out vec4 fragColor;

in vec2 f_texCoord;

uniform uvec3 tint;

layout(binding=0) uniform sampler2D tex;
void main()
{                          
    fragColor = texture(tex, f_texCoord) * vec4(tint.r / 255.0, tint.g / 255.0, tint.b / 255.0,1.0);
}
