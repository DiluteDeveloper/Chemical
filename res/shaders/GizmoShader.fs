#version 460 core

out vec4 fragColor;

uniform vec3 gizmoColor;

void main()
{
  	
    fragColor = vec4(gizmoColor, 1.0);
}