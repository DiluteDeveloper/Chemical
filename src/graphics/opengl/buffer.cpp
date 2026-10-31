#include <GLES3/gl3.h>

#include "buffer.h"

namespace Chemical {

	namespace OpenGL {

		namespace {
			// GL_ELEMENT_ARRAY_BUFFER binding is part of the *currently bound VAO*, so uploading index
			// data must not leave a different buffer attached to whatever VAO happens to be bound.
			struct ScopedBufferBind {
				GLenum target;
				GLint previous = 0;
				bool restore;

				ScopedBufferBind(GLenum target, GLuint id) :
					target(target), restore(target == GL_ELEMENT_ARRAY_BUFFER) {
					if (restore)
						glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &previous);
					glBindBuffer(target, id);
				}
				~ScopedBufferBind() {
					if (restore)
						glBindBuffer(target, (GLuint)previous);
				}
			};
		}

		Buffer::Buffer(BufferTarget target) :
			target(target) {
			glGenBuffers(1, &rendererID);
		}
		Buffer::~Buffer() {
			Release();
		}
		void Buffer::Release() {
			if (rendererID != 0) {
				glDeleteBuffers(1, &rendererID);
				rendererID = 0;
			}
		}
		void Buffer::SetBufferData(int64_t size, const void* data, uint32_t offset) {
			ScopedBufferBind bind((GLenum)target, rendererID);
			glBufferSubData((GLenum)target, (GLintptr)offset, (GLsizeiptr)size, data);
		}
		void Buffer::CreateMutableBuffer(int64_t size, const void* data, BufferDataFlags flags) {
			ScopedBufferBind bind((GLenum)target, rendererID);
			glBufferData((GLenum)target, (GLsizeiptr)size, data, (GLenum)flags);
		}
		void Buffer::CreateImmutableBuffer(int64_t size, const void* data, BufferStorageFlags flags) {
			const GLenum usage = ((flags & BufferStorageFlags::DYNAMIC_STORAGE_BIT) != BufferStorageFlags::NONE)
				? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
			ScopedBufferBind bind((GLenum)target, rendererID);
			glBufferData((GLenum)target, (GLsizeiptr)size, data, usage);
		}

		void Buffer::BindBufferBase(BufferBaseTarget baseTarget, uint32_t bindingIndex) {
			// glBindBufferBase also binds to the generic target, no separate glBindBuffer needed.
			glBindBufferBase((GLenum)baseTarget, bindingIndex, rendererID);
		}

	}

}
