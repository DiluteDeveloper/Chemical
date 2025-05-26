Chemical 1.1.6

Added transform component to StaticMesh and StaticMeshTraits,
applied to mat3 uniform in shader. No renderer indexable array yet,
will be implemented later on as doesnt make sense for renderer to contain transforms.
added testing scenario to main for circling mesh transform

Chemical 1.1.5

changed api directory to include 
added imported modules directory to sandbox include 
paths as a hacky fix for glm 
added image loading function and struct using stb
added quick test of image loading function in core.cpp
changed all but static mesh object IDs to be strings instead
of integers
combined renderer static mesh vector to shader id array and
renderer static mesh array into one ShaderStaticMeshes struct referred
to by shaderid
returning std::nullopt instead of nullptr for std::optional; bugfix
added materials and material string ids that have an albedo attribute,
added test in sandbox main.cpp to test this functionality
changed shader uniform set functions to const

Chemical 1.1.3

Renamed mesh.h and mesh.cpp files to static_mesh.h and static_mesh.cpp
Added various logs
Added new 2D renderer
Added sandbox tests for 2D renderer
Renamed ShaderProgram class and files to Shader;
StaticMesh and Shader now use {Class name}Traits structs for their constructors
Removed "Hello World" print in
Patched up sandbox test code 
Added triangle 2D shape
Modified resources.h
Added .ignore to gitignore

Chemical 1.1.2

Added shader loading backend module,
currently being used in sandbox for testing
renamed chemical include folder to api to reflect
actual purpose

Chemical 1.1.1

Switched to Linux development 
Added support for shapes Square and Triangle
