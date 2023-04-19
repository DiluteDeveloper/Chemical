#version 460 core

out vec4 fragColor;

in vec2 f_texCoord;
/*flat in uint f_materialIndex;

layout(std140, binding = 0) uniform Material
{
    vec4 diffColour[1024];
    vec4 specColour[1024];
} material;*/

void main()
{                          

    //fragColor = material.diffColour[f_materialIndex] * material.specColour[f_materialIndex];
    fragColor = vec4(1.0,1.0,1.0,1.0);
}