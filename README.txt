todo:

remove vertex layout from shaderprogram class
swap to std::arrays
add last portion of faces bottom faces and top

notes:

Chunk blocks could be stored only if they are rendered. they are being implemented so all the blocks in a chunk are going to be actually created and stored in memory. for a 128x128x128 area that is 25mb of data for just a 3 component vector for each block. this could become quite expensive in the future so keep in mind.