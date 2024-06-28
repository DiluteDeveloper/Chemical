#version 460 core

out vec4 fragColor;

in vec2 f_vertex_tex_coord;

uniform sampler2D image_texture;

void main()
{                          

    fragColor = texture(image_texture, f_vertex_tex_coord);
}