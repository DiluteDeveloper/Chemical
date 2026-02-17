Game Development 0.19.0

Implemented kepler orbit physics.
todo: add physics manipulation gui,
add orbit trails,
add more than 2 celestial bodies (if possible)

Game Development 0.18.8

I spent all weekend trying to procedurally generate
a smooth shaded isosphere mesh (without any help other than
looking into basic math functions like sin) and I've finally done it!
I'm gonna go enjoy the little bit of my weekend I have left.

Game Development 0.18.8_i

Intermediate commit; Created
flat-shaded Icosphere generation code
and working on smooth shaded Icosphere generation code;
Doing a rewrite of smooth shaded

Game Development 0.18.7

In the middle of optimizing Marching Cubes,
pivoting to universe sim and icosphere mesh gen

Game Development 0.18.6

Added noise octaves and fixed bug that made perlin noise 
segfault sometimes (out of bounds)

Game Development 0.18.5

Removed key input and mouse input logging
added Perlin Noise seeding, activated with random seed
via Regenerate terrain button
Fixed camera moving when in menu mode

Game Development 0.18.4

Changed window stuff

Game Development 0.18.3

Created TerrainState class for managing state of perlin noise
frequency and amplitude as well as terrain size.
Added new GUI menu to control values in TerrainState
Renamed Perlin.hpp gui class to terrain_state_menu
added back deletion of vertex buffers
GenerateMarchingCubes doesnt do noise anymore, takes in an IsoGrid
where the noise has already been generated

Game Development 0.18.2

Fixed lighting
Readded GUI, made a start on adding perlin noise (and marching cubes)
GUI configuration to game

Game Development 0.18.1

Rendering marching cubes with normals

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
