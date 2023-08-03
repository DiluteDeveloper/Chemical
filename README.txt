changes this commit:
Changed chunk shader input to correctly reflect variables, normal to colour.
chunk shader now takes in chunk origin vector instead of model matrix.
Massively simplified noise generation.
Removed relative coordinates in rendering to support switching to origin set in shader.
main now gives a random number as the seed to the chunk.

todo:

remove vertex layout from shaderprogram class
switch chunks to heap

notes:

Chunk blocks could be stored only if they are rendered. they are being implemented so all the blocks in a chunk are going to be actually created and stored in memory. for a 128x128x128 area that is 25mb of data for just a 3 component vector for each block. this could become quite expensive in the future so keep in mind.