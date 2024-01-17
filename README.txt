![A screenshot from the engine](github_image.png "A screenshot from the engine"))

changes this commit:

added textures for stone, dirt, grass and bedrock blocks as well as giving texture coordinates to the generated vertex data.
added commented code as started working towards having every chunk contained in a single SSBO but this may be dropped.
lowered render distance and camera speed for testing
stb_image integration properly used as well as texture opengl class

todo:

add a new thread for chunk generation - on the backburner

try using vertex buffer binding index instead of reallocating

known issues:

memory leak came back

seems that chunks on the render distance edge doesnt generate edge faces.

notes:

have observed blocks getting cut off at high y levels but not all the same level.
not sure on this.

BIT LAYOUT FOR CHUNK BLOCK DATA
5,5,5,5 5,5,5,5 5,5,5,5 4,4,4,4 4,3,3,3 3,3,3,3 2,2,2,2 2,1,1,1


1 = face orientation( see below )

5 = block type
4 = x position within chunk
3 = y position within chunk
2 = z position within chunk;
2 = position within chunk

FACES:

000/0 = TOP
001/1 = BOTTOM
010/2 = RIGHT
011/3 = LEFT
100/4 = FRONT
101/5 = BACK
110/6
111/7


Not sure if DYNAMIC_DRAW is the best flag for chunk mesh vertex buffers.


shader texID refers to which texture the current block/mesh/pixel is using.

0 = dirt
1 = grass
2 = stone
3 = bedrock