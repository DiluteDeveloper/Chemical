#pragma once

namespace OpenGL {


	enum class DataType {
		BYTE = 0x1400,
		UNSIGNED_BYTE = 0x1401,
		SHORT = 0x1402,
		UNSIGNED_SHORT = 0x1403,
		INT = 0x1404,
		UNSIGNED_INT = 0x1405,
		FLOAT = 0x1406,
		HALF_FLOAT = 0x140B,
		DOUBLE = 0x140A,
		FIXED = 0x140C,
		INT_2_10_10_10_REV = 0x8D9F,
		UNSIGNED_INT_2_10_10_10_REV = 0x8368,
		UNSIGNED_INT_10F_11F_11F_REV = 0x8C3B,
		UNSIGNED_BYTE_3_3_2 = 0x8032,
		UNSIGNED_BYTE_2_3_3_REV = 0x8362,
		UNSIGNED_SHORT_5_6_5 = 0x8363,
		UNSIGNED_SHORT_5_6_5_REV = 0x8364,
		UNSIGNED_SHORT_4_4_4_4 = 0x8033,
		UNSIGNED_SHORT_4_4_4_4_REV = 0x8365,
		UNSIGNED_SHORT_5_5_5_1 = 0x8034,
		UNSIGNED_SHORT_1_5_5_5_REV = 0x8366,
		UNSIGNED_INT_8_8_8_8 = 0x8035,
		UNSIGNED_INT_8_8_8_8_REV = 0x8367,
		UNSIGNED_INT_10_10_10_2 = 0x8036
	};

	// returns 0 if type isnt found
	extern uint64_t ConvertEnumDataTypeToByteSize(DataType type);
	// returns 0 if type isnt found
	extern uint64_t ConvertUnsignedIntegralDataTypeToByteSize(DataType type);

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
