#include <GLES3/gl3.h>
#include <spdlog/spdlog.h>

#include "misc.h"


namespace Chemical {

	namespace OpenGL {

		uint64_t ConvertEnumDataTypeToByteSize(DataType type) {
			switch (type) {
			case DataType::BYTE:
				return sizeof(int8_t);
			case DataType::UNSIGNED_BYTE:
				return sizeof(uint8_t);
			case DataType::SHORT:
				return sizeof(int16_t);
			case DataType::UNSIGNED_SHORT:
				return sizeof(uint16_t);
			case DataType::INT:
				return sizeof(int32_t);
			case DataType::UNSIGNED_INT:
				return sizeof(uint32_t);
			case DataType::FLOAT:
				return sizeof(float);
			case DataType::HALF_FLOAT:
				return sizeof(uint16_t);
			case DataType::FIXED:
				return sizeof(int32_t);
			case DataType::INT_2_10_10_10_REV:
			case DataType::UNSIGNED_INT_2_10_10_10_REV:
			case DataType::UNSIGNED_INT_10F_11F_11F_REV:
			case DataType::UNSIGNED_INT_5_9_9_9_REV:
			case DataType::UNSIGNED_INT_24_8:
				return sizeof(uint32_t);
			case DataType::FLOAT_32_UNSIGNED_INT_24_8_REV:
				return 8;
			case DataType::UNSIGNED_SHORT_5_6_5:
			case DataType::UNSIGNED_SHORT_4_4_4_4:
			case DataType::UNSIGNED_SHORT_5_5_5_1:
				return sizeof(uint16_t);
			default:
				return 0;
			}
		}

		uint64_t ConvertUnsignedIntegralDataTypeToByteSize(DataType type) {
			switch (type) {
			case DataType::UNSIGNED_BYTE:
				return sizeof(uint8_t);
			case DataType::UNSIGNED_SHORT:
				return sizeof(uint16_t);
			case DataType::UNSIGNED_INT:
				return sizeof(uint32_t);
			default:
				return 0;
			}
		}

		void VertexLayout::AddAttribute(const VertexAttribute& attribute) {
			stride += static_cast<int32_t>(attribute.components * ConvertEnumDataTypeToByteSize(attribute.dataType));

			attributes.emplace_back(attribute);
		}

		const std::vector<VertexAttribute>& VertexLayout::GetAttributes() const {
#ifdef CHEMICAL_DEBUG
			if (attributes.empty())
				spdlog::warn("VertexLayout GetAttributes() is being called when VertexLayout does not contain any attributes.");
#endif
			return attributes;
		}

		int32_t VertexLayout::GetStride() const {
#ifdef CHEMICAL_DEBUG
			if (stride == 0)
				spdlog::warn("VertexLayout GetStride() is being called when stride is 0.");
#endif
			return stride;
		}
	}
}
