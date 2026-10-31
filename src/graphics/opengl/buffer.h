#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

namespace Chemical {


	namespace OpenGL {

		// Without DSA a buffer has to be bound to a target to be touched, and WebGL2 wants a
		// buffer to always be used with the same kind of target (index buffers must be
		// ELEMENT_ARRAY_BUFFER, never anything else). So the target is chosen up front.
		enum class BufferTarget {
			ARRAY_BUFFER = 0x8892,              // vertex data
			ELEMENT_ARRAY_BUFFER = 0x8893,      // index data
			UNIFORM_BUFFER = 0x8A11,
			TRANSFORM_FEEDBACK_BUFFER = 0x8C8E
		};

		// glBufferStorage doesn't exist in GLES 3.0. CreateImmutableBuffer is emulated, only
		// DYNAMIC_STORAGE_BIT is honoured (it selects DYNAMIC_DRAW instead of STATIC_DRAW).
		enum class BufferStorageFlags {
			NONE = 0,
			DYNAMIC_STORAGE_BIT = 0x0100
		};

		// Flags for dynamic buffers, how the data will be accessed
		enum class BufferDataFlags {

			STREAM_DRAW = 0x88E0,
			STREAM_READ = 0x88E1,
			STREAM_COPY = 0x88E2,
			STATIC_DRAW = 0x88E4,
			STATIC_READ = 0x88E5,
			STATIC_COPY = 0x88E6,
			DYNAMIC_DRAW = 0x88E8,
			DYNAMIC_READ = 0x88E9,
			DYNAMIC_COPY = 0x88EA
		};

		// Atomic counter and shader storage buffers are GLES 3.1+, not available in WebGL2.
		enum class BufferBaseTarget {
			TRANSFORM_FEEDBACK_BUFFER = 0x8C8E,
			UNIFORM_BUFFER = 0x8A11
		};

		inline BufferStorageFlags operator & (BufferStorageFlags lhs, BufferStorageFlags rhs)
		{
			using T = std::underlying_type_t <BufferStorageFlags>;
			return static_cast<BufferStorageFlags>(static_cast<T>(lhs) & static_cast<T>(rhs));
		}
		inline BufferStorageFlags operator | (BufferStorageFlags lhs, BufferStorageFlags rhs)
		{
			using T = std::underlying_type_t <BufferStorageFlags>;
			return static_cast<BufferStorageFlags>(static_cast<T>(lhs) | static_cast<T>(rhs));
		}

		// Recommended to make a shared_ptr as copy constructor is disabled
		class Buffer {
			uint32_t rendererID = 0;
			BufferTarget target = BufferTarget::ARRAY_BUFFER;

			void Release();
		public:

			// Creates a buffer name. The data store is created in CreateImmutableBuffer() or CreateMutableBuffer().
			// target must match how the buffer is used (vertex data = ARRAY_BUFFER, indices = ELEMENT_ARRAY_BUFFER, ...).
			explicit Buffer(BufferTarget target = BufferTarget::ARRAY_BUFFER);

			Buffer(const Buffer&) = delete;
			Buffer(Buffer&& other) noexcept :
				rendererID(std::exchange(other.rendererID, 0)), target(other.target) { }
			Buffer& operator=(const Buffer&) = delete;
			Buffer& operator=(Buffer&& other) noexcept {
				if (this != &other) {
					Release();
					rendererID = std::exchange(other.rendererID, 0);
					target = other.target;
				}
				return *this;
			}

			~Buffer();

			// Will set the buffer data to data at offset with byte size of size
			void SetBufferData(int64_t size, const void* data = nullptr, uint32_t offset = 0);
			// Will create new data store that can be resized with the same function and fills it with data,
			// calling this will empty out any data previously contained within the buffer.
			void CreateMutableBuffer(int64_t size, const void* data = nullptr, BufferDataFlags flags = BufferDataFlags::STATIC_DRAW);
			// Emulated "immutable" buffer (GLES 3.0 has no glBufferStorage): allocates the store once with
			// glBufferData. Don't call twice.
			void CreateImmutableBuffer(int64_t size, const void* data = nullptr, BufferStorageFlags flags = BufferStorageFlags::DYNAMIC_STORAGE_BIT);

			// Only valid for UNIFORM_BUFFER / TRANSFORM_FEEDBACK_BUFFER.
			void BindBufferBase(BufferBaseTarget target, uint32_t bindingIndex);

			uint32_t GetRendererID() const {
				return rendererID;
			}
			BufferTarget GetTarget() const {
				return target;
			}

		};


	}

}
