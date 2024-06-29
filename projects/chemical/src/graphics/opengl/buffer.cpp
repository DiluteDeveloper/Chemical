#include "pch.h"

#include <GLAD/glad.h>

#include "buffer.h"


namespace Chemical {

	namespace OpenGL {

		Buffer::Buffer() {
			glCreateBuffers(1, &renderer_id);
		}
		Buffer::~Buffer() {
			glDeleteBuffers(1, &renderer_id);
		}
		void Buffer::SetBufferData(int64_t size, const void* data, uint32_t offset) {
			glNamedBufferSubData(renderer_id, offset, size, data);
		}
		void Buffer::CreateMutableBuffer(int64_t size, const void* data, BufferDataFlags flags) {
			glNamedBufferData(renderer_id, size, data, (GLenum)flags);
		}
		void Buffer::CreateImmutableBuffer(int64_t size, const void* data, BufferStorageFlags flags) {
			glNamedBufferStorage(renderer_id, size, data, (GLbitfield)flags);
		}

		void Buffer::BindBufferBase(BufferBaseTarget target, uint32_t binding_index) {

			glBindBuffer((GLenum)target, renderer_id);
			glBindBufferBase((GLenum)target, binding_index, renderer_id); // may not work like a tall
		}

	}

}

