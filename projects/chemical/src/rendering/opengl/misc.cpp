#include <pch.h>
#include <GLAD/glad.h>
#include <spdlog/spdlog.h>

#include "misc.h"


namespace Chemical {


	namespace OpenGL {

		uint64_t ConvertEnumDataTypeToByteSize(DataType type) {
			switch (type) {
			case DataType::BYTE:
				return sizeof(GLbyte);
				break;
			case DataType::UNSIGNED_BYTE:
				return sizeof(GLubyte);
				break;
			case DataType::SHORT:
				return sizeof(GLshort);
				break;
			case DataType::UNSIGNED_SHORT:
				return sizeof(GLushort);
				break;
			case DataType::INT:
				return sizeof(GLint);
				break;
			case DataType::UNSIGNED_INT:
				return sizeof(GLuint);
				break;
			case DataType::FLOAT:
				return sizeof(GLfloat);
				break;
			case DataType::HALF_FLOAT:
				return sizeof(GLhalf);
				break;
			case DataType::DOUBLE:
				return sizeof(GLdouble);
				break;
			case DataType::FIXED:
				return sizeof(GLfixed);
				break;
			case DataType::INT_2_10_10_10_REV:
				return sizeof(GLuint);
				break;
			case DataType::UNSIGNED_INT_2_10_10_10_REV:
				return sizeof(GLuint);
				break;
			case DataType::UNSIGNED_INT_10F_11F_11F_REV:
				return sizeof(GLuint);
				break;
			default:
				return 0;
			}

		}

		extern uint64_t ConvertUnsignedIntegralDataTypeToByteSize(DataType type) {
			switch (type) {
			case DataType::UNSIGNED_BYTE:
				return sizeof(GLubyte);
				break;
			case DataType::UNSIGNED_SHORT:
				return sizeof(GLushort);
				break;
			case DataType::UNSIGNED_INT:
				return sizeof(GLuint);
				break;
			default:
				return 0;
			}
		}

		void VertexLayout::AddAttribute(const VertexAttribute& attribute) {
			stride += static_cast<int32_t>(attribute.components * ConvertEnumDataTypeToByteSize(attribute.dataType));

			attributes.emplace_back(attribute);
		}

		const std::vector<VertexAttribute>& VertexLayout::GetAttributes() const {
			if (attributes.size() == 0)
				spdlog::warn("VertexLayout GetAttributes() is being called when VertexLayout does not contain any attributes.");
			return attributes;
		}

		int32_t VertexLayout::GetStride() const {
			if (stride == 0)
				spdlog::warn("VertexLayout GetStride() is being called when stride is 0.");
			return stride;
		}
	}
}
