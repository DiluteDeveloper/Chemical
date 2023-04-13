#pragma once

#include <glad/glad.h>

namespace OpenGL {

	struct VertexAttribute {

		uint32_t m_offset = 0;
		uint32_t m_size = 0;

		uint32_t m_components = 0;

		enum class DataType {
			BYTE = 0x1400,
			UNSIGNED_BYTE = 0x1401,
			SHORT = 0x1402,
			UNSIGNED_SHORT = 0x1403,
			INT = 0x1404,
			UNSIGNED_INT = 0x1405,
			FLOAT = 0x1406,
			HALF_FLOAT = 0x140B,
			DOUBLE = 0x140A
		} m_dataType;

		enum class DataTransformation {
			DONT_NORMALIZE_OUTPUT_FLOAT = 0,
			NORMALIZE_OUTPUT_FLOAT = 1,
			OUTPUT_INTEGRAL = 2,
			OUTPUT_DOUBLE = 3,
		} m_dataTransformation = DataTransformation::DONT_NORMALIZE_OUTPUT_FLOAT;

		VertexAttribute(uint32_t components, DataType dataType, DataTransformation dataTransformation = DataTransformation::DONT_NORMALIZE_OUTPUT_FLOAT) :
			m_components(components), m_dataTransformation(dataTransformation), m_dataType(dataType) {

			switch (dataType) {
			case DataType::BYTE:
				m_size = static_cast<uint32_t>(sizeof(GLbyte)) * m_components;
				break;
			case DataType::UNSIGNED_BYTE:
				m_size = static_cast<uint32_t>(sizeof(GLubyte)) * m_components;
				break;
			case DataType::SHORT:
				m_size = static_cast<uint32_t>(sizeof(GLshort)) * m_components;
				break;
			case DataType::UNSIGNED_SHORT:
				m_size = static_cast<uint32_t>(sizeof(GLushort)) * m_components;
				break;
			case DataType::INT:
				m_size = static_cast<uint32_t>(sizeof(GLint)) * m_components;
				break;
			case DataType::UNSIGNED_INT:
				m_size = static_cast<uint32_t>(sizeof(GLuint)) * m_components;
				break;
			case DataType::FLOAT:
				m_size = static_cast<uint32_t>(sizeof(GLfloat)) * m_components;
				break;
			case DataType::HALF_FLOAT:
				m_size = static_cast<uint32_t>(sizeof(GLhalf)) * m_components;
				break;
			case DataType::DOUBLE:
				m_size = static_cast<uint32_t>(sizeof(GLdouble)) * m_components;
				break;
			}
		}

	};

	struct VertexAttributeLayout {
		std::vector<VertexAttribute> m_attributes;
		uint32_t m_stride = 0;

		VertexAttributeLayout(std::initializer_list<VertexAttribute> attributes) {
			m_attributes.reserve(attributes.size());
			for (const VertexAttribute& va : attributes) {
				VertexAttribute va2(va);
				va2.m_offset = m_stride;
				m_stride += va2.m_size;
				m_attributes.emplace_back(va2);
			}
		}
	};

	// struct is temporary
	class Mesh {
		uint32_t m_rendererID;

		Mesh(uint32_t rendererID) :
			m_rendererID(rendererID) {}
	public:

		void Draw(uint32_t count) {
			glBindVertexArray(m_rendererID);
			glDrawArrays(GL_TRIANGLES, 0, count);
		}

		// Follows RAII rules best it can
		template<typename T>
		static std::shared_ptr<Mesh> CreateStaticMesh(const std::vector<T> vertexData, const VertexAttributeLayout& layout) {

			uint32_t rendererID = 0;
			uint32_t vbID = 0;
			glCreateBuffers(1, &vbID);
			glCreateVertexArrays(1, &rendererID);

			glNamedBufferStorage(vbID, sizeof(T) * vertexData.size(), vertexData.data(), GL_MAP_READ_BIT);
			glVertexArrayVertexBuffer(rendererID, 0, vbID, 0, layout.m_stride);

			size_t i = 0;
			for (const VertexAttribute& va : layout.m_attributes) {

				switch (va.m_dataTransformation) {
				case VertexAttribute::DataTransformation::DONT_NORMALIZE_OUTPUT_FLOAT:
					[[fallthrough]];
				case VertexAttribute::DataTransformation::NORMALIZE_OUTPUT_FLOAT:
					glVertexArrayAttribFormat(rendererID, i, va.m_components, (GLenum)va.m_dataType, (GLboolean)va.m_dataTransformation, va.m_offset);
					break;
				case VertexAttribute::DataTransformation::OUTPUT_INTEGRAL:
					glVertexArrayAttribIFormat(rendererID, i, va.m_components, (GLenum)va.m_dataType, va.m_offset);
					break;
				case VertexAttribute::DataTransformation::OUTPUT_DOUBLE:
					glVertexArrayAttribLFormat(rendererID, i, va.m_components, (GLenum)va.m_dataType, va.m_offset);
					break;
				}
				glVertexArrayAttribBinding(rendererID, i, 0);
				glEnableVertexArrayAttrib(rendererID, i);


				i++;
			}
			return std::shared_ptr<Mesh>(new Mesh(rendererID));
		}


	};
}

