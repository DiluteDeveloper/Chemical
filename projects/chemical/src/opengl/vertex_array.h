#pragma once

#include "buffer.h"
#include "misc.h"

namespace OpenGL {

	enum class DrawMode {
		POINTS = 0x0000,
		LINE_STRIP = 0x0003,
		LINE_LOOP = 0x0002,
		LINES = 0x0001,
		LINE_STRIP_ADJACENCY = 0x000B,
		LINES_ADJACENCY = 0x000A,
		TRIANGLE_STRIP = 0x0005,
		TRIANGLE_FAN = 0x0006,
		TRIANGLES = 0x0004,
		TRIANGLE_STRIP_ADJACENCY = 0x000D,
		TRIANGLES_ADJACENCY = 0x000C,
		PATCHES = 0x000E
	};

	// Encodes how OpenGL will transform your data.
	enum class DataTransformation {
		NORMALIZED_INT_TO_FLOAT = 0, // data is a normalized int, converts data to float
		INT_TO_FLOAT = 1,  // data is an int, converts data to float
		INT = 2, // data is an int, keeps data as int
		DOUBLE = 3, // data is a double, keeps data as double
		FLOAT = 4 // data is a float, keeps data as float
	};

	struct VertexAttribute {
		DataType dataType = DataType::FLOAT;
		DataTransformation transformation = DataTransformation::FLOAT;

		uint32_t offset = 0;

		int32_t components = 0;

		VertexAttribute() = default;
		VertexAttribute(int32_t components, uint32_t offset, DataType dataType = DataType::FLOAT, DataTransformation transformation = DataTransformation::FLOAT) :
			components(components), offset(offset), dataType(dataType), transformation(transformation) {}
	};
	
	struct InstancedElementDrawInfo {
		DrawMode mode = DrawMode::TRIANGLES;
		DataType dataType = DataType::UNSIGNED_INT; // must be uint, ushort, ubyte
		uint32_t count = 0;
		int64_t offset = 0;
		uint32_t instanceCount = 0;

		InstancedElementDrawInfo(uint32_t count, uint32_t instanceCount, int64_t offset = 0, DataType dataType = DataType::UNSIGNED_INT, DrawMode mode = DrawMode::TRIANGLES) :
			count(count), instanceCount(instanceCount), offset(offset), dataType(dataType), mode(mode) {}
		InstancedElementDrawInfo() = default;
	};
	struct ElementDrawInfo {
		DrawMode mode = DrawMode::TRIANGLES;
		DataType dataType = DataType::UNSIGNED_INT; // must be uint, ushort, ubyte
		uint32_t count = 0;
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
		uint32_t count = 0;
		int32_t first = 0;

		ArrayDrawInfo(uint32_t count, uint32_t first = 0, DrawMode mode = DrawMode::TRIANGLES) :
			count(count), first(first), mode(mode) {}
		ArrayDrawInfo() = default;
	};


	class VertexBufferInfo {

		std::vector<VertexAttribute> m_attributes;

	public:

		VertexBufferInfo(int32_t stride, int64_t offset, uint32_t bindingIndex) :
			stride(stride), offset(offset), bindingIndex(bindingIndex) {}

		void AddAttribute(const VertexAttribute& attribute) {
			m_attributes.emplace_back(attribute);
		}

		const std::vector<VertexAttribute>& GetAttributes() const {
			return m_attributes;
		}

		int32_t stride = 0;
		int64_t offset = 0;
		uint32_t bindingIndex = 0;
	};

	// Only supports 1 vertex buffer
	class VertexArray {
		uint32_t m_rendererID = 0;
	public:

		VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		~VertexArray();

		void SetVertexBuffer(const Buffer& buffer, const VertexBufferInfo& info);
		void SetElementBuffer(const Buffer& buffer);

		void DrawElements(const ElementDrawInfo& info);
		void DrawArrays(const ArrayDrawInfo& info);

		void DrawElementsInstanced(const InstancedElementDrawInfo& info);
		void DrawArraysInstanced(const InstancedArrayDrawInfo& info);

		void Bind() const;
		void Unbind() const;
	};
}
