Alpha 1.10.1

changes this commit:

discovered memory leak never existed, app just happens to slightly increase in usage for the first few 10/100s of chunks that get loaded

spent maybe 5 hours fixing a bug where chunk Z+ and X+ edges wouldnt appear upon loading new chunks in the X+, Z- and X- direction only on the lowest Z chunks.
Turns out it was just some chunk mesh buffer code inside of the wrong brackets.

added doChunkLoading (enable with 7, disable with 6, enabled default) for debugging purposes. do not go more than 1 chunk outside of where you turned it off or game
will crash due to how the chunkloader is designed.

moved center_origin alignment code in chunkloader update to be inside of if statements, small optimization

small optimization: moved 

todo:

add a new thread for chunk generation - on the backburner

try using vertex buffer binding index instead of reallocating

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