#version 460 core

out vec4 fragColor;

in vec2 f_texCoord;
flat in uint f_materialIndex;

struct MaterialData {

    vec4 diffColour;
    vec4 specColour;
};

layout(std140, binding = 0) uniform Material
{
    MaterialData data[1024];
} material;

void main()
{                          

    fragColor = material.data[f_materialIndex].diffColour * material.data[f_materialIndex].specColour;
    //fragColor = vec4(1.0,1.0,1.0,1.0);
}