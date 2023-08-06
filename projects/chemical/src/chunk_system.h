#pragma once

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/shader_program.h"

#include <vector>
#include <glm/glm.hpp>

#define CHUNK_SIZE_X 128
#define CHUNK_SIZE_Y 128
#define CHUNK_SIZE_Z 128

namespace Chemical {


	void InitializeBlockData(); // needs to be called before any chunk generation code

	using BlockID = uint16_t;


	struct ChunkVertex {
		glm::fvec3 position;
		glm::fvec3 normal;

		ChunkVertex(const glm::fvec3& position, const glm::fvec3 normal) :
			position(position), normal(normal) {}
	};

	struct ChunkMesh {
		OpenGL::VertexArray v_array;
		OpenGL::ArrayDrawInfo info;

	};

	class Chunk
	{
		friend class ChunkLoader; friend class ChunkRenderer; friend class std::unique_ptr<Chunk> std::make_unique<Chunk>();

		ChunkMesh mesh;

		// x,z,z block origin is at -x-y-z
		std::array<BlockID, CHUNK_SIZE_X* CHUNK_SIZE_Y* CHUNK_SIZE_Z> blocks = { 0 };

		// chunk origin is at -x-z
		glm::ivec2 origin = glm::ivec2(0);

		Chunk() {}; // private constructor, can only be created as unique_ptr on heap

	public:

		static std::unique_ptr<Chunk> CreateChunk(const glm::ivec2& origin, uint32_t seed);

	};
	class ChunkRenderer {

		std::unique_ptr<OpenGL::ShaderProgram> chunk_shader;
	public:

		ChunkRenderer();

		void RenderChunk(const std::unique_ptr<Chunk>& chunk);

		// More optimized than rendering 1 chunk at a time due to shader binding
		void RenderChunks(const std::vector<std::unique_ptr<Chunk>>& chunks);

		void GenerateChunkMesh(const std::unique_ptr<Chunk>& chunk);
	};


	// loads in chunks around a position(generally the player)
	class ChunkLoader {

		friend class std::unique_ptr<ChunkLoader> std::make_unique<ChunkLoader>();

		ChunkRenderer renderer;

		std::vector<std::unique_ptr<Chunk>> loaded_chunks;

		uint8_t render_distance = 8;
		uint32_t seed = 8;

		ChunkLoader() {}; // private constructor, can only be created as unique_ptr on heap

	public:

		static std::unique_ptr<ChunkLoader> CreateChunkLoader(uint8_t render_distance, uint32_t seed);
		static std::unique_ptr<ChunkLoader> CreateChunkLoader(uint32_t seed);

		void Update();
	};



}
