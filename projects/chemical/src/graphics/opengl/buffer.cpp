#include "pch.h"

#include <GLAD/glad.h>

#include "buffer.h"

namespace OpenGL {
	
	Buffer::Buffer() {
		glCreateBuffers(1, &m_rendererID);
	}
	Buffer::~Buffer() {
		//glDeleteBuffers(1, &m_rendererID);
	}
	void Buffer::SetBufferData(int64_t size, const void* data, uint32_t offset) {
		glNamedBufferSubData(m_rendererID, offset, size, data);
	}
	void Buffer::CreateMutableBuffer(int64_t size, const void* data, BufferDataFlags flags) {
		glNamedBufferData(m_rendererID, size, data, (GLenum)flags);
	}
	void Buffer::CreateImmutableBuffer(int64_t size, const void* data, BufferStorageFlags flags) {
		glNamedBufferStorage(m_rendererID, size, data, (GLbitfield)flags);
	}

	void Buffer::BindBufferBase(BufferBaseTarget target, uint32_t bindingIndex) {
		glBindBufferBase((GLenum)target, bindingIndex, m_rendererID); // may not work like a tall
		glBindBuffer((GLenum)target, m_rendererID);
	}

}
