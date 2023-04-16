#include "pch.h"

#include <GLAD/glad.h>

#include "buffer.h"

namespace OpenGL {
	
	Buffer::Buffer() {
		glCreateBuffers(1, &m_rendererID);
	}
	Buffer::~Buffer() {
		glDeleteBuffers(1, &m_rendererID);
	}
	void Buffer::SetBufferData(uint32_t size, void* data, uint32_t offset) {
		glNamedBufferSubData(m_rendererID, offset, size, data);
	}
	void Buffer::CreateMutableBuffer(uint32_t size, void* data, BufferDataFlags flags) {
		glNamedBufferData(m_rendererID, size, data, (GLenum)flags);
	}
	void Buffer::CreateImmutableBuffer(uint32_t size, void* data, BufferStorageFlags flags) {
		glNamedBufferStorage(m_rendererID, size, data, (GLbitfield)flags);
	}

}
