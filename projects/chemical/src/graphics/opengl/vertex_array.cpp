#include "pch.h"
#include <glad/glad.h>

#include "vertex_array.h"


namespace Chemical {
	namespace OpenGL {

		VertexArray::VertexArray() {
			glCreateVertexArrays(1, &renderer_id);
		}

		void VertexArray::SetVertexBuffer(const Buffer& buffer, const VertexLayout& info, int64_t offset, uint32_t binding_index) {
			glVertexArrayVertexBuffer(renderer_id, binding_index, buffer.GetRendererID(), offset, info.GetStride());
			uint32_t i = 0;
			for (const VertexAttribute& att : info.GetAttributes())
			{
				switch (att.transformation) {
				case DataTransformation::INT:
					glVertexArrayAttribIFormat(renderer_id, i, att.components, (GLenum)att.data_type, (GLuint)att.offset);
					break;
				case DataTransformation::NORMALIZED_INT_TO_FLOAT:
					glVertexArrayAttribFormat(renderer_id, i, att.components, (GLenum)att.data_type, GL_TRUE, (GLuint)att.offset);
					break;
				case DataTransformation::INT_TO_FLOAT:
					[[fallthrough]];
				case DataTransformation::FLOAT:
					glVertexArrayAttribFormat(renderer_id, i, att.components, (GLenum)att.data_type, GL_FALSE, (GLuint)att.offset);
					break;
				case DataTransformation::DOUBLE:
					glVertexArrayAttribLFormat(renderer_id, i, att.components, (GLenum)att.data_type, (GLuint)att.offset);
					break;
				}
				glEnableVertexArrayAttrib(renderer_id, i);
				glVertexArrayAttribBinding(renderer_id, i, binding_index);

				i++;
			}
		}
		void VertexArray::SetElementBuffer(const Buffer& buffer) {
			glVertexArrayElementBuffer(renderer_id, buffer.GetRendererID());
		}

		void VertexArray::Bind() const {
			glBindVertexArray(renderer_id);
		}
		void VertexArray::Unbind() const {
			glBindVertexArray(0);
		}
		void VertexArray::DrawElements(const ElementDrawInfo& info) const {
			glDrawElements((GLenum)info.mode, info.count, (GLenum)info.data_type, (void*)info.offset);
		}
		void VertexArray::DrawArrays(const ArrayDrawInfo& info)  const {
			glDrawArrays((GLenum)info.mode, info.first, info.count);
		}

		void VertexArray::DrawElementsInstanced(const InstancedElementDrawInfo& info) const {
			glDrawElementsInstanced((GLenum)info.mode, info.count, (GLenum)info.data_type, (void*)info.offset, info.instance_count);
		}
		void VertexArray::DrawArraysInstanced(const InstancedArrayDrawInfo& info) const {
			glDrawArraysInstanced((GLenum)info.mode, info.first, info.count, info.instance_count);
		}

		VertexArray::~VertexArray() {
			glDeleteVertexArrays(1, &renderer_id);
		}
	}
}


