changes this commit:
remove vertex layout from shaderprogram class todo removed ( keeping it for now)
ChunkRender was renamed to ChunkMesh
ChunkRender member vArray was renamed to v_array
blockData was renamed to block_registry
Implemented BlockType enum to be used to index into block_registry
dimension function renamed to Dimension
dimension parameters changed from int to size_t
SetBlockData renamed to InitializeBlockData

todo:


notes:
