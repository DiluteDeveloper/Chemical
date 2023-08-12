changes this commit:
Fixed memory leak,
erasure was not happening of the significant coordinate(X) of the chunk maps in the x offset update code.

vertex buffer size is reduced 6 fold due to bit manipulation, 6*6 max vertices are sent to the shader(planned to be reduced to 1*6)
and each vertice is only 1 32 bit uint to represent position, colour, block type, etc.
went from 6*6*24 bytes to 6*6*4

Fixed camera inverting when mouse moving too far down or up

todo:

add a new thread for chunk generation - on the backburner

try using vertex buffer binding index instead of reallocating

memory leak came back

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