#pragma once

#include <cstdint>
#include <utility>

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
				mode(mode), dataType(dataType), count(count), offset(offset), instanceCount(instanceCount) {}
			InstancedElementDrawInfo() = default;
		};
		struct ElementDrawInfo {
			DrawMode mode = DrawMode::TRIANGLES;
			DataType dataType = DataType::UNSIGNED_INT; // must be uint, ushort, ubyte
			int32_t count = 0;
			int64_t offset = 0;

			ElementDrawInfo(uint32_t count, int64_t offset = 0, DataType dataType = DataType::UNSIGNED_INT, DrawMode mode = DrawMode::TRIANGLES) :
				mode(mode), dataType(dataType), count((int32_t)count), offset(offset) {}
			ElementDrawInfo() = default;
		};
		struct InstancedArrayDrawInfo {
			DrawMode mode = DrawMode::TRIANGLES;
			uint32_t count = 0;
			int32_t first = 0;
			uint32_t instanceCount = 0;

			InstancedArrayDrawInfo(uint32_t count, uint32_t instanceCount, int32_t first = 0, DrawMode mode = DrawMode::TRIANGLES) :
				mode(mode), count(count), first(first), instanceCount(instanceCount) {}
			InstancedArrayDrawInfo() = default;
		};
		struct ArrayDrawInfo {
			DrawMode mode = DrawMode::TRIANGLES;
			int32_t count = 0;
			int32_t first = 0;

			ArrayDrawInfo(int32_t count, uint32_t first = 0, DrawMode mode = DrawMode::TRIANGLES) :
				mode(mode), count(count), first((int32_t)first) {}
			ArrayDrawInfo() = default;
		};


		// A VAO remembers which buffers its attributes read from, so a vertex buffer can be
		// deleted once it is attached (it lives on until the VAO lets go of it).
		//
		// NOTE: without DSA, SetVertexBuffer/SetElementBuffer have to bind this VAO, and they
		// leave it bound afterwards.
		class VertexArray {
			uint32_t rendererID = 0;

			void Release();
		public:

			VertexArray();

			VertexArray(VertexArray&& other) noexcept :
				rendererID(std::exchange(other.rendererID, 0)) {}
			VertexArray(const VertexArray&) = delete;
			VertexArray& operator=(const VertexArray&) = delete;
			VertexArray& operator=(VertexArray&& other) noexcept {
				if (this != &other) {
					Release();
					rendererID = std::exchange(other.rendererID, 0);
				}
				return *this;
			}

			~VertexArray();

			// buffer must be an ARRAY_BUFFER buffer.
			// bindingIndex is unused (vertex binding points need GL 4.3); kept so call sites don't change.
			// Attribute locations are assigned 0..N-1 in layout order, so shaders need layout(location = N).
			void SetVertexBuffer(const Buffer& buffer, const VertexLayout& info, int64_t offset, uint32_t bindingIndex);
			// buffer must be an ELEMENT_ARRAY_BUFFER buffer.
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
