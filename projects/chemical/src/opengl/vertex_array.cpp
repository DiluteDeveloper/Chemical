#include "pch.h"
#include <glad/glad.h>

#include "vertex_array.h"

namespace OpenGL {

	VertexArray::VertexArray() {
		glCreateVertexArrays(1, &m_rendererID);
	}

	void VertexArray::SetVertexBuffer(const Buffer& buffer, const VertexBufferInfo& info) {
		glVertexArrayVertexBuffer(m_rendererID, info.bindingIndex, buffer.GetRendererID(), info.offset, info.stride);
		uint32_t i = 0;
		for (const VertexAttribute& att : info.GetAttributes())
		{
			switch (att.transformation) {
			case DataTransformation::INT:
				glVertexArrayAttribIFormat(m_rendererID, i, att.components, (GLenum)att.dataType, (GLuint)att.offset);
				break;
			case DataTransformation::NORMALIZED_INT_TO_FLOAT:
				glVertexArrayAttribFormat(m_rendererID, i, att.components, (GLenum)att.dataType, GL_TRUE, (GLuint)att.offset);
				break;
			case DataTransformation::INT_TO_FLOAT:
				[[fallthrough]];
			case DataTransformation::FLOAT:
				glVertexArrayAttribFormat(m_rendererID, i, att.components, (GLenum)att.dataType, GL_FALSE, (GLuint)att.offset);
				break;
			case DataTransformation::DOUBLE:
				glVertexArrayAttribLFormat(m_rendererID, i, att.components, (GLenum)att.dataType, (GLuint)att.offset);
				break;
			}
			glEnableVertexArrayAttrib(m_rendererID, i);
			glVertexArrayAttribBinding(m_rendererID, i, info.bindingIndex);

			i++;
		}
	}
	void VertexArray::SetElementBuffer(const Buffer& buffer) {
		glVertexArrayElementBuffer(m_rendererID, buffer.GetRendererID());
	}

	void VertexArray::Bind() const {
		glBindVertexArray(m_rendererID);
	}
	void VertexArray::Unbind() const {
		glBindVertexArray(0);
	}
	void VertexArray::DrawElements(const ElementDrawInfo& info) {
		glDrawElements((GLenum)info.mode, info.count, (GLenum)info.dataType, (void*)info.offset);
	}
	void VertexArray::DrawArrays(const ArrayDrawInfo& info) {
		glDrawArrays((GLenum)info.mode, info.first, (GLsizei)info.count);
	}

	void VertexArray::DrawElementsInstanced(const InstancedElementDrawInfo& info) {
		glDrawElementsInstanced((GLenum)info.mode, info.count, (GLenum)info.dataType, (void*)info.offset, info.instanceCount);
	}
	void VertexArray::DrawArraysInstanced(const InstancedArrayDrawInfo& info) {
		glDrawArraysInstanced((GLenum)info.mode, info.first, info.count, info.instanceCount);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_rendererID);
	}
}