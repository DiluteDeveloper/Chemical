#pragma once

#include "buffer.h"
#include "misc.h"


namespace Chemical {


	namespace OpenGL {

		struct InstancedElementDrawInfo {
			DrawMode mode = DrawMode::TRIANGLES;
			DataType dataType = DataType::UNSIGNED_INT; // must be uint, ushort, ubyte
			int32_t count = 0;
			int64_t offset = 0;
			int32_t instanceCount = 0;

			InstancedElementDrawInfo(int32_t count, int32_t instanceCount, int64_t offset = 0, DataType dataType = DataType::UNSIGNED_INT, DrawMode mode = DrawMode::TRIANGLES) :
				count(count), instanceCount(instanceCount), offset(offset), dataType(dataType), mode(mode) {}
			InstancedElementDrawInfo() = default;
		};
		struct ElementDrawInfo {
			DrawMode mode = DrawMode::TRIANGLES;
			DataType dataType = DataType::UNSIGNED_INT; // must be uint, ushort, ubyte
			int32_t count = 0;
			int64_t offset = 0;

			ElementDrawInfo(uint32_t count, int64_t offset = 0, DataType dataType = DataType::UNSIGNED_INT, DrawMode mode = DrawMode::TRIANGLES) :
				count(count), offset(offset), dataType(dataType), mode(mode) {}
			ElementDrawInfo() = default;
		};
		struct InstancedArrayDrawInfo {
			DrawMode mode = DrawMode::TRIANGLES;
			uint32_t count = 0;
			int32_t first = 0;
			uint32_t instanceCount = 0;

			InstancedArrayDrawInfo(uint32_t count, uint32_t instanceCount, int32_t first = 0, DrawMode mode = DrawMode::TRIANGLES) :
				count(count), instanceCount(instanceCount), first(first), mode(mode) {}
			InstancedArrayDrawInfo() = default;
		};
		struct ArrayDrawInfo {
			DrawMode mode = DrawMode::TRIANGLES;
			int32_t count = 0;
			int32_t first = 0;

			ArrayDrawInfo(int32_t count, uint32_t first = 0, DrawMode mode = DrawMode::TRIANGLES) :
				count(count), first(first), mode(mode) {}
			ArrayDrawInfo() = default;
		};



		// http://alex-charlton.com/posts/When_is_it_okay_to_delete_an_OpenGL_buffer/
		/*
		According to the OpenGL specification, you can, for instance,
		reference a vertex buffer object (VBO) in a vertex array object (VAO)
		and then immediately delete the VBO. The VAO will retain the reference to the VBO,
		which will only truly get deleted when all references to it are deleted.
		The relevant parts from the spec that explain this are section 5.1.2:
		*/
		// tl:dr vertex arrays contain the equivalent of shared_ptr's to buffers, they will only truly
		// be deleted when they are no longer referenced.
		class VertexArray {
			uint32_t rendererID = 0;

		public:

			VertexArray();

			VertexArray(VertexArray&& other) noexcept :
				rendererID(std::move(other.rendererID)) {
				other.rendererID = 0;
			}
			VertexArray(const VertexArray&) = delete;
			VertexArray& operator=(const VertexArray&) = delete;
			VertexArray& operator=(VertexArray&& other) noexcept {
				this->rendererID = std::move(other.rendererID);
				other.rendererID = 0;
				return *this;
			}

			~VertexArray();

			void SetVertexBuffer(const Buffer& buffer, const VertexLayout& info, int64_t offset, uint32_t bindingIndex);
			void SetElementBuffer(const Buffer& buffer);

			void DrawElements(const ElementDrawInfo& info) const;
			void DrawArrays(const ArrayDrawInfo& info) const;

			void DrawElementsInstanced(const InstancedElementDrawInfo& info) const;
			void DrawArraysInstanced(const InstancedArrayDrawInfo& info) const;

			void Bind() const;
			void Unbind() const;
		};
	}

}
