#include "graphics/gl.h"

#include <cstdint>

#include "vertex_array.h"


namespace Chemical {
	namespace OpenGL {

		VertexArray::VertexArray() {
			glGenVertexArrays(1, &rendererID);
		}

		void VertexArray::SetVertexBuffer(const Buffer& buffer, const VertexLayout& info, int64_t offset, uint32_t bindingIndex) {
			(void)bindingIndex;

			glBindVertexArray(rendererID);
			glBindBuffer(GL_ARRAY_BUFFER, buffer.GetRendererID());

			const GLsizei stride = (GLsizei)info.GetStride();
			uint32_t i = 0;
			for (const VertexAttribute& att : info.GetAttributes())
			{
				// glVertexAttribPointer's last argument is the byte offset into the bound buffer
				const void* pointer = reinterpret_cast<const void*>(static_cast<uintptr_t>(offset + att.offset));

				switch (att.transformation) {
				case DataTransformation::INT:
					glVertexAttribIPointer(i, att.components, (GLenum)att.dataType, stride, pointer);
					break;
				case DataTransformation::NORMALIZED_INT_TO_FLOAT:
					glVertexAttribPointer(i, att.components, (GLenum)att.dataType, GL_TRUE, stride, pointer);
					break;
				case DataTransformation::INT_TO_FLOAT:
				case DataTransformation::FLOAT:
					glVertexAttribPointer(i, att.components, (GLenum)att.dataType, GL_FALSE, stride, pointer);
					break;
				}
				glEnableVertexAttribArray(i);

				i++;
			}

			// The VAO has captured the pointers; the generic binding isn't VAO state.
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		void VertexArray::SetElementBuffer(const Buffer& buffer) {
			// element array binding is VAO state, so this VAO must be bound (and stays bound)
			glBindVertexArray(rendererID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.GetRendererID());
		}

		void VertexArray::Bind() const {
			glBindVertexArray(rendererID);
		}
		void VertexArray::Unbind() const {
			glBindVertexArray(0);
		}
		void VertexArray::DrawElements(const ElementDrawInfo& info) const {
			glDrawElements((GLenum)info.mode, info.count, (GLenum)info.dataType,
				reinterpret_cast<const void*>(static_cast<uintptr_t>(info.offset)));
		}
		void VertexArray::DrawArrays(const ArrayDrawInfo& info)  const {
			glDrawArrays((GLenum)info.mode, info.first, info.count);
		}

		void VertexArray::DrawElementsInstanced(const InstancedElementDrawInfo& info) const {
			glDrawElementsInstanced((GLenum)info.mode, info.count, (GLenum)info.dataType,
				reinterpret_cast<const void*>(static_cast<uintptr_t>(info.offset)), info.instanceCount);
		}
		void VertexArray::DrawArraysInstanced(const InstancedArrayDrawInfo& info) const {
			glDrawArraysInstanced((GLenum)info.mode, info.first, info.count, info.instanceCount);
		}

		void VertexArray::Release() {
			if (rendererID != 0) {
				glDeleteVertexArrays(1, &rendererID);
				rendererID = 0;
			}
		}
		VertexArray::~VertexArray() {
			Release();
		}
	}
}
