Game Development 0.18.0

Came back after a long break.
Removed alot of testing code for collisions,
focusing on marching cube algorithm now;
Understood and implemented a perlin noise algorithm adapted
from a C# blog found online; created test scene with cubes to visualise

Game Development 0.17.5

No longer calculating redundant axes for separating axis theorem
Slightly changed SAT boolean logic
Slightly changed AABB variables
Added collision testing selection to GUI
Changes to main function and loaded model/s

Game Development 0.17.4

Added event based input
Made the escape key to exit and enter menu mode event based instead of hold key based
Stopped the camera from moving when in menu mode
Fixed camera position changing when leaving menu mode

Game Development 0.17.3

Changed meshes to have transform data instead of 4x4 matrix and changed all functions referencing it
Added boilerplate IMGUI code to main function
Created new GUI function that takes in a list of transform references and puts them in a GUI window to be modified via position, rotation, and scale
Submitted all transforms currently in the application to the new GUI function
Added name component to mesh to be used in GUI function and loaded name from assimp model loading

Game Development 0.17.2

Added model matrix to mesh, SAT in action
in new test scene by pressing X

Game Development 0.17.1

Added Separating Axis Theorem functionality

Game Development 0.17.0

Added some basic box collision detection
and changed mesh workflow

Game Development 0.16.0

Added support for multiple meshes in a single model
Modified shader to have assertions to protect against 
invalid shader uniforms

Game Development 0.15.0

Simplified meta run scripts
loading materials ambient, diffuse,
specular, and shininess from 3D models 
into shader
ASSIMP STATIC LIBRARY SHOULD BE IN modules/imported/assimp
THIS LIBRARY IS NOT ON GITHUB (TOO LARGE).

Game Development 0.14.0 

Added new ModelLoader header and TU
for loading 3D models using assimp;
loaded stanford dragon
added model matrix to shader and main function
added assimp static library to build

Game Development 0.13.0 

Added normals and basic LearnOpenGL lighting
grabbed from previous project

Game Development 0.12.0

Added 3D free camera controller
Added projection matrix

Game Development 0.11.0

Boilerplate stuff

Game Development 0.1.0

Starting from scratch again
