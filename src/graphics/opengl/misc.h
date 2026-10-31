#pragma once

#include <cstdint>
#include <vector>

namespace Chemical {

	namespace OpenGL {

		// Only types that are valid in GLES 3.0 / WebGL2 (GL_DOUBLE and the
		// desktop-only packed types were removed).
		enum class DataType {
			BYTE = 0x1400,
			UNSIGNED_BYTE = 0x1401,
			SHORT = 0x1402,
			UNSIGNED_SHORT = 0x1403,
			INT = 0x1404,
			UNSIGNED_INT = 0x1405,
			FLOAT = 0x1406,
			HALF_FLOAT = 0x140B,
			FIXED = 0x140C,
			INT_2_10_10_10_REV = 0x8D9F,
			UNSIGNED_INT_2_10_10_10_REV = 0x8368,
			UNSIGNED_INT_10F_11F_11F_REV = 0x8C3B,
			UNSIGNED_INT_5_9_9_9_REV = 0x8C3E,
			UNSIGNED_INT_24_8 = 0x84FA,
			FLOAT_32_UNSIGNED_INT_24_8_REV = 0x8DAD,
			UNSIGNED_SHORT_5_6_5 = 0x8363,
			UNSIGNED_SHORT_4_4_4_4 = 0x8033,
			UNSIGNED_SHORT_5_5_5_1 = 0x8034
		};

		// returns 0 if type isnt found
		extern uint64_t ConvertEnumDataTypeToByteSize(DataType type);
		// returns 0 if type isnt found
		extern uint64_t ConvertUnsignedIntegralDataTypeToByteSize(DataType type);

		// Adjacency modes and PATCHES are not available in GLES 3.0 / WebGL2.
		enum class DrawMode {
			POINTS = 0x0000,
			LINES = 0x0001,
			LINE_LOOP = 0x0002,
			LINE_STRIP = 0x0003,
			TRIANGLES = 0x0004,
			TRIANGLE_STRIP = 0x0005,
			TRIANGLE_FAN = 0x0006
		};

		// Encodes how OpenGL will transform your data.
		// (DOUBLE was removed: glVertexAttribLPointer doesn't exist in GLES/WebGL.)
		enum class DataTransformation {
			NORMALIZED_INT_TO_FLOAT = 0, // data is a normalized int, converts data to float
			INT_TO_FLOAT = 1,  // data is an int, converts data to float
			INT = 2, // data is an int, keeps data as int
			FLOAT = 4 // data is a float, keeps data as float
		};

		struct VertexAttribute {
			DataType dataType = DataType::FLOAT;
			DataTransformation transformation = DataTransformation::FLOAT;

			uint32_t offset = 0;

			int32_t components = 0;

			VertexAttribute() = default;
			VertexAttribute(int32_t components, uint32_t offset, DataType dataType = DataType::FLOAT, DataTransformation transformation = DataTransformation::FLOAT) :
				dataType(dataType), transformation(transformation), offset(offset), components(components) {}
		};

		class VertexLayout {

			std::vector<VertexAttribute> attributes;

			int32_t stride = 0;

		public:

			VertexLayout(int64_t offset, uint32_t bindingIndex) {}
			VertexLayout() {}

			void AddAttribute(const VertexAttribute& attribute);

			const std::vector<VertexAttribute>& GetAttributes() const;

			int32_t GetStride() const;
		};

	}

}
