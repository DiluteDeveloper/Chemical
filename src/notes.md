# Idea

Want this branch to be a parkour sort of game inspired by minecraft parkour maps

# Level editor

I want to have a level editor but I want to keep it simple in regards to
development so I will just make it so that the level editor lets you put down block faces
instead of whole blocks so greedy meshing algorithms don't need to be added.

# Rendering BlockGroups

each block type should have its own drawcall. each block face should upload
it's 3D position, and a x,y,z integer offset for where it ends to allow
to combine multiple faces into one, basically manual greedy meshing. technically
this would allow for diagonal faces which could be nice. Also, the face needs
an orientation.

The face position should always be the lowest x,y,z position in the group
such that the size offsets are always positive.

ivec3 face_position;
int face_data { 
    
    first 8 bits = 256 different block types
    next 7 bits = max of 128 x offset
    next 7 bits = max of 128 y offset
    next 7 bits = max of 128 z offset
    last 3 bits = face orientation
}




std::vector<std::vector<Cell>> cells; 

BlockType current_block_type;

for x in cells.size():
    for y in cells[x].size():
        
        current_block_type = cells[x][y].block_type;

        if(cells[x + 1][y].block_type == current_block_type) {
            
        }
        
