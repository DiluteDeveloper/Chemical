Alpha 1.02

OpenGL object wrapper move operations now set rendererID to 0 for
moved-from object, to prevent deleting an active 
OpenGL object on deconstruction.

Added texture coordinates and texture to 
sprite, renderer and shader workflow.
Switched to model, view, projection 
matrix for positioning sprites.

Added window_size variable to Core

Added SpriteRenderer for cleanliness and workflow

Changed Sprite2D::InitializeSprite2D() to
spriteRenderer.initializeSpriteRenderer() but essentially
the same functionality

Added basic image loading to sprites

Modified Util::LoadImage and Util::ImageData to be accurate

todo:

currently move operations on opengl wrapper types delete opengl objects of ID 0. this is possible overhead.