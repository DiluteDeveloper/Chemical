#pragma once

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/shader_program.h"

#include <vector>
#include <glm/glm.hpp>

// limit of 255 due to uint8_t iterators.
// determines the X and Z dimensions of every chunk.
// do not change without modifying shader code.
#define CHUNK_SIZE 16

// no easily reachable limit.
// determines the y dimension of a chunk used for iterating over.
// do not change without modifying shader code.
#define CHUNK_HEIGHT 128

namespace Chemical {

	// a named index into the block registry in chunk_shader.
	enum BlockType {
		Air = 0,
		Stone = 1,
		Dirt = 2,
		Grass = 3,
		Bedrock = 4
	};

	struct ChunkVertex {
		uint32_t bit_data = 0;

		ChunkVertex(uint32_t bit_data) :
			bit_data(bit_data) {} 
		ChunkVertex() {}
	};

	// needs to be removed for SSBO type storage, on the backburner
	struct ChunkMesh {
		
		OpenGL::Buffer v_buffer;

		// the current byte allocation of v_buffer
		uint32_t v_buffer_size = 0;
		OpenGL::VertexArray v_array;
		OpenGL::ArrayDrawInfo info;
		
	};

	class Chunk;

	// used for rendering chunk edges
	// nullptr represents a non-existent chunk edge
	struct ChunkEdges {
		Chunk* zp_chunk = nullptr;
		Chunk* zm_chunk = nullptr;
		Chunk* xp_chunk = nullptr;
		Chunk* xm_chunk = nullptr;
	};

	class Chunk
	{
		friend class ChunkLoader; friend class ChunkRenderer;

		ChunkMesh mesh;
		//std::vector<uint32_t> faces; 	// needs to be added for SSBO type storage, on the backburner
		ChunkEdges edges;

		// stores BlockType aka an identifier for which block is at which position within the chunk.
		// stored positions go from (0 - (CHUNK_SIZE - 1)) X and Z (0 - (CHUNK_HEIGHT - 1)) Y
		// when these blocks are rendered the index represents the negative corner of the block,
		// as in the block vertices go from X, Y, Z to X + 1, Y + 1, Z + 1
		std::array<BlockType, (CHUNK_SIZE*CHUNK_SIZE) * CHUNK_HEIGHT> blocks = { BlockType::Air};

	public:

		// generates the blocks based on the seeded perlin noise heightmap
		// origin is in chunk coordinates, not multiplied by CHUNK_SIZE
		// chunks are very large and should be heap allocated when possible
		// seed is used for random perlin noise heightmap generation
		Chunk(const glm::ivec2& origin, uint32_t seed);

	};

	class ChunkRenderer {

		std::unique_ptr<OpenGL::ShaderProgram> chunk_shader;

		// contains all chunks faces
		//OpenGL::Buffer chunk_ssbo;
		//OpenGL::VertexArray dummyVArray;
	public:

		// performs setup of chunk_shader and its projection matrix
		ChunkRenderer();

		// renders given list of chunks mapped to their [X][Z] chunk coordinates.
		void RenderChunks(const std::unordered_map<int32_t, std::unordered_map<int32_t, std::unique_ptr<Chunk>>>& chunks);


		// assigns the v_array vertex buffer to v_buffer and applies chunk_shader vertex layout
		void SetupChunkMesh(const std::unique_ptr<Chunk>& chunk);



		// rebuilds/builds the chunk mesh and its edges if any.
		// can reallocate a new buffer if new vertices byte size exceeds v_buffer_size.
		// allocation_multiplier indicates the multiplier
		// for the size of the new buffer if an allocation is required.
		// allocation_multiplier must be greater or equal to 1.0f.
		void BuildChunkMesh(const std::unique_ptr<Chunk>& chunk, float allocation_multiplier = 1.0f);

		// used in BuildChunkMeshEdges when v_buffer needs to be reallocated
		// to rebuild/build the chunk mesh with no chunk edges, to then buffer
		// the edges after as subdata
		// allocation_multiplier indicates the multiplier
		// for the size of the new buffer if an allocation is required.
		// allocation_multiplier must be greater or equal to 1.0f.
		void BuildChunkMeshNoEdges(const std::unique_ptr<Chunk>& chunk, float allocation_multiplier = 1.0f);

		// will build/rebuild all the chunk mesh edges.
		// will not rebuild the chunk mesh itself unless the new buffer
		// size exceeds v_buffer_size and a new allocation needs to be done.
		// allocation_multiplier indicates the multiplier
		// for the size of the new buffer if an allocation is required.
		// allocation_multiplier must be greater or equal to 1.0f.
		void UpdateChunkMeshEdges(const std::unique_ptr<Chunk>& chunk, float allocation_multiplier = 1.0f);
	};

	// loads in chunks based on player_transform 
	class ChunkLoader {

		ChunkRenderer renderer;


		// chunk coordinate mapping [x][z] of all loaded and rendered chunks
		std::unordered_map<int32_t, std::unordered_map<int32_t, std::unique_ptr<Chunk>>> loaded_chunks;

		// center chunk coordinate of loaded_chunks
		glm::ivec2 center_origin = glm::ivec2(0);

		// the chunk distance from the middle chunk
		uint16_t render_distance = 0;

		uint32_t seed = 0;

	public:

		bool doChunkLoading = true;

		// loads new chunks in a region around the player
		ChunkLoader(uint32_t seed, uint8_t render_distance = 8);

		// loads new chunks if needed and renders them
		void Update();
	};



}
