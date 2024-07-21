#include "pch.h"

#include <GLAD/glad.h>

#include "buffer.h"


namespace Chemical {

	namespace OpenGL {

		Buffer::Buffer() {
			glCreateBuffers(1, &rendererID);
		}
		Buffer::~Buffer() {
			 //glDeleteBuffers(1, &rendererID);
		}
		void Buffer::SetBufferData(int64_t size, const void* data, uint32_t offset) {
			glNamedBufferSubData(rendererID, offset, size, data);
		}
		void Buffer::CreateMutableBuffer(int64_t size, const void* data, BufferDataFlags flags) {
			glNamedBufferData(rendererID, size, data, (GLenum)flags);
		}
		void Buffer::CreateImmutableBuffer(int64_t size, const void* data, BufferStorageFlags flags) {
			glNamedBufferStorage(rendererID, size, data, (GLbitfield)flags);
		}

		void Buffer::BindBufferBase(BufferBaseTarget target, uint32_t bindingIndex) {

			glBindBuffer((GLenum)target, rendererID);
			glBindBufferBase((GLenum)target, bindingIndex, rendererID); // may not work like a tall
		}

	}

}

