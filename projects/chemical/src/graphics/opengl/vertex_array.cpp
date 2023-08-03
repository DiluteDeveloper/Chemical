#include "pch.h"
#include <glad/glad.h>

#include "vertex_array.h"


namespace Chemical {
	namespace OpenGL {

		VertexArray::VertexArray() {
			glCreateVertexArrays(1, &rendererID);
		}

		void VertexArray::SetVertexBuffer(const Buffer& buffer, const VertexLayout& info, int64_t offset, uint32_t bindingIndex) {
			glVertexArrayVertexBuffer(rendererID, bindingIndex, buffer.GetRendererID(), offset, info.GetStride());
			uint32_t i = 0;
			for (const VertexAttribute& att : info.GetAttributes())
			{
				switch (att.transformation) {
				case DataTransformation::INT:
					glVertexArrayAttribIFormat(rendererID, i, att.components, (GLenum)att.dataType, (GLuint)att.offset);
					break;
				case DataTransformation::NORMALIZED_INT_TO_FLOAT:
					glVertexArrayAttribFormat(rendererID, i, att.components, (GLenum)att.dataType, GL_TRUE, (GLuint)att.offset);
					break;
				case DataTransformation::INT_TO_FLOAT:
					[[fallthrough]];
				case DataTransformation::FLOAT:
					glVertexArrayAttribFormat(rendererID, i, att.components, (GLenum)att.dataType, GL_FALSE, (GLuint)att.offset);
					break;
				case DataTransformation::DOUBLE:
					glVertexArrayAttribLFormat(rendererID, i, att.components, (GLenum)att.dataType, (GLuint)att.offset);
					break;
				}
				glEnableVertexArrayAttrib(rendererID, i);
				glVertexArrayAttribBinding(rendererID, i, bindingIndex);

				i++;
			}
		}
		void VertexArray::SetElementBuffer(const Buffer& buffer) {
			glVertexArrayElementBuffer(rendererID, buffer.GetRendererID());
		}

		void VertexArray::Bind() const {
			glBindVertexArray(rendererID);
		}
		void VertexArray::Unbind() const {
			glBindVertexArray(0);
		}
		void VertexArray::DrawElements(const ElementDrawInfo& info) const {
			glDrawElements((GLenum)info.mode, info.count, (GLenum)info.dataType, (void*)info.offset);
		}
		void VertexArray::DrawArrays(const ArrayDrawInfo& info)  const {
			glDrawArrays((GLenum)info.mode, info.first, info.count);
		}

		void VertexArray::DrawElementsInstanced(const InstancedElementDrawInfo& info) const {
			glDrawElementsInstanced((GLenum)info.mode, info.count, (GLenum)info.dataType, (void*)info.offset, info.instanceCount);
		}
		void VertexArray::DrawArraysInstanced(const InstancedArrayDrawInfo& info) const {
			glDrawArraysInstanced((GLenum)info.mode, info.first, info.count, info.instanceCount);
		}

		VertexArray::~VertexArray() {
			glDeleteVertexArrays(1, &rendererID);
		}
	}
}


