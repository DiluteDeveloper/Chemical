Chemical 1.2.6_int

Intermediary update amongst a rewrite of the
entire rendering pipeline, scene class and JSON loading.
Temporarily disabled user scripting.

Chemical 1.2.5

Rudimentary Scene loading from JSON implemented with nholmann-json
Alot of code needs some cleanup
Moved/added public deleted copy ctor/op and added default move ctor/op to Scene
Switched shader fstream file read to ifstream
uncommented GlDeleteBuffers
Changed core to load scene from json, removed scene asset loading code from Scene ctor
as well as user code in Player.cpp and Background.cpp

Chemical 1.2.4

Added scene scripts for user to control the scene.
Completely changed build methodology; sandbox headers
and TUs are incorporated into chemical executable.
I.E Entry point is now controlled by Chemical, not Sandbox
Added new SafePtr type (T* typedef) to represent pointer values that are not null;
This was needed to fix include loops happening that cant be fixed with
references but unsure if the type will stay, just on trial for now

Chemical 1.2.3

Added GLFW error callback to window
Added window title and window size to core constructor
Renamed StaticMesh to StaticSprite and removed API functionality for
custom vertices and indices; only supply size of rect to describe mesh;
removed StaticMeshTraits struct
renamed shader_mapped_static_meshes to static_sprites

Chemical 1.2.2

Added new Camera class with zooming functionality,
projection matrix and view matrix for moving and zooming
the camera
Added new functionality to scene to create and add cameras,
and set primary camera
renamed transform.h GetTransform() to CreateMat3()
added projection matrix and view matrix to shader
added new physics to testing scenario with zooming via Z and X keys

Chemical 1.2.1

Added new Input class with very basic functionality
to get keyboard keys (basically a glfwGetKey wrapper).
Input class instance is accessed in Core via GetInput()
Added new function StartGame(Core& core) to main.cpp
to mitigate a segfault on termination of core;
Issue is due to opengl objects created in main being destroyed at end
of program after opengl context is unloaded in ~Core 
Removed Terminate() from Core as well as do_terminate.
GameLoop now returns an int with -1 being used to terminate the game loop.
Terminate functionality is now in Core dtor.
Added a character controller demo to main.cpp to test chemical api capabilities

Chemical 1.2

Completely overhauled Scene
Deprecated ResourceManager
Change to C++ dialect 23
Added a fair few code comments
Added very verbose logging system throughout
Renamed albedo to tint in material and added
default argument constructor
Made opengl object naming consistent with naming 
object identifier variables gl_id
Changed static mesh traits constructor
Changed indice_count to idx_count in static mesh
Scene now constructs the default shader object in ctor
Changed main.cpp to reflect changes but kept same testing
scenario as 1.1.9.5

Chemical 1.1.9.5

Added ResourceManager

Chemical 1.1.9

Implemented new Scene system and new renderer SceneRenderer
Resources are stored in Scene for now till ResourceManager
is implemented in the next update

Chemical 1.1.8

Changed Core module into a class
Put Renderer object onto Core module
Fixed a bug related to renderer
Gameloop is now contained in Core and is subscribed to. see main.cpp
Removed misc.h and misc.cpp and put functionality in Core

Chemical 1.1.7

Added support for textures in renderer and texture traits and shader
renamed image_loader.h to image.h 
removed test image load in core
definitely needs refactoring

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
