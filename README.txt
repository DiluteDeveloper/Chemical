changes this commit:
Added 2 more blocks being Air block and Bedrock block with id 0 and 4 respectively
Block data is now stored in a 1D array using a function called dimension in chunk_system.cpp to convert 3D to 1D
BlockID is now a using macro instead of a struct
perlin noise, chunk size was changed once again, perlin noise y can go from 5-25y
block data now stores air blocks instead of a dynamic sized Y array, air blocks being value initialized to 0 in blocks array
memory usage issues are now fixed and each BlockID is 2 bytes each

todo:

remove vertex layout from shaderprogram class
chunk block positions are stored as uint64_t's because of being std::array indexes. too much data. many wow
create enum for block types
i dont like the amount of if statements happening in RenderChunk
clean up block type determination in chunk generation function
add macro or define for bottom chunk Y so it isnt always hardcoded 0, same for X and Z possibly

notes:

Chunk blocks could be stored only if they are rendered. they are being implemented so all the blocks in a chunk are going to be actually created and stored in memory. for a 128x128x128 area that is 25mb of data for just a 3 component vector for each block. this could become quite expensive in the future so keep in mind.

keep in mind access patterns for organization of data