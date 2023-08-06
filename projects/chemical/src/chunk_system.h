#pragma once

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/shader_program.h"

#include <vector>
#include <glm/glm.hpp>

#define CHUNK_SIZE_X 16 // MAX OF 64
#define CHUNK_SIZE_Y 72
#define CHUNK_SIZE_Z 16 // MAX OF 64

namespace Chemical {


	void InitializeBlockData(); // needs to be called before any chunk generation code

	enum BlockType {
		Air = 0,
		Stone = 1,
		Dirt = 2,
		Grass = 3,
		Bedrock = 4
	};

	struct ChunkVertex {
		glm::fvec3 position = glm::fvec3(0);
		glm::fvec3 colour = glm::fvec3(1);

		ChunkVertex(const glm::fvec3& position, const glm::fvec3 colour) :
			position(position), colour(colour) {}
		ChunkVertex() {}
	};

	struct ChunkMesh {
		
		OpenGL::Buffer v_buffer;
		uint32_t v_buffer_size = 0;
		OpenGL::VertexArray v_array;
		OpenGL::ArrayDrawInfo info;

	};

	class Chunk
	{
		friend class ChunkLoader; friend class ChunkRenderer;

		ChunkMesh mesh;

		// x,z,z block origin is at -x-y-z
		std::array<BlockType, CHUNK_SIZE_X* CHUNK_SIZE_Y* CHUNK_SIZE_Z> blocks = { BlockType::Air};

		// chunk origin is at -x-z 
		glm::ivec2 origin = glm::ivec2(0);

	public:
		Chunk(const glm::ivec2& origin, uint32_t seed); // should be heap allocated

	};
	class ChunkRenderer {

		std::unique_ptr<OpenGL::ShaderProgram> chunk_shader;
	public:

		ChunkRenderer();

		void RenderChunk(const std::unique_ptr<Chunk>& chunk);

		// More optimized than rendering 1 chunk at a time due to shader binding
		void RenderChunks(const std::unordered_map<int32_t, std::unordered_map<int32_t, std::unique_ptr<Chunk>>>& chunks);

		void GenerateChunkMesh(const std::unique_ptr<Chunk>& chunk);
		// 0 = X-, 1 = Z+, 2 = X+, 3 = Z-;
		void GenerateChunkMeshEdged(const std::unique_ptr<Chunk>& chunk, const std::array<const Chunk*, 4> edges);

		void RegenerateChunkMesh(const std::unique_ptr<Chunk>& chunk);
	};


	// loads in chunks around a position(generally the player)
	class ChunkLoader {

		ChunkRenderer renderer;

		std::unordered_map<int32_t, std::unordered_map<int32_t, std::unique_ptr<Chunk>>> loaded_chunks;

		glm::ivec2 center_chunk_origin = glm::ivec2(0);

		uint8_t real_rd = 8;
		uint32_t seed = 8;

	public:

		ChunkLoader(uint32_t seed, uint8_t render_distance = 8);

		void Update();
	};



}
