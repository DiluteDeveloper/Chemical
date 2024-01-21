Alpha 1.10.4 World Manipulation

changes this commit:

added very basic lighting
changed perlin noise generation
added 2 more bits to the Y value of the chunk block data and removed 2 bits from block type and offset accordingly.
Increased chunk height to 400

known issues:

world manipulation aka block removing and placing causes chunk edge rendering issues, touched upon in comments near remove block and
place block function declaration
world manipulation positioning is miscalculated for chunks in negative axes, touched upon in comments remove block and
place block function declaration

todo:

add a new thread for chunk generation - on the backburner

try using vertex buffer binding index instead of reallocating

notes:

have observed blocks getting cut off at high y levels but not all the same level.
not sure on this.

BIT LAYOUT FOR CHUNK BLOCK DATA
5,5,5,5 5,5,5,5 5,5,4,4 4,4,4,3 3,3,3,3 3,3,3,3 2,2,2,2 2,1,1,1


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