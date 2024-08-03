#pragma once


namespace Chemical {


	namespace OpenGL {

		// Bitfield Flags for buffer storage, only has & operator implemented
		enum class BufferStorageFlags {
			DYNAMIC_STORAGE_BIT = 0x0100,
			MAP_READ_BIT = 0x0001,
			MAP_WRITE_BIT = 0x0002,
			MAP_PERSISTENT_BIT = 0x0040,
			MAP_COHERENT_BIT = 0x0080,
			CLIENT_STORAGE_BIT = 0x0200
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

		enum class BufferBaseTarget {
			ATOMIC_COUNTER_BUFFER = 0x92C0,
			TRANSFORM_FEEDBACK_BUFFER = 0x8C8E,
			UNIFORM_BUFFER = 0x8A11,
			SHADER_STORAGE_BUFFER = 0x90D2
		};

		inline BufferStorageFlags operator & (BufferStorageFlags lhs, BufferStorageFlags rhs)
		{
			using T = std::underlying_type_t <BufferStorageFlags>;
			return static_cast<BufferStorageFlags>(static_cast<T>(lhs) & static_cast<T>(rhs));
		}

		// Recommended to make a shared_ptr as copy constructor is disabled
		class Buffer {
			uint32_t rendererID = 0;
		public:


			// Creates a buffer rendererID object. data store can be created in CreateImmutableBuffer() or CreateMutableBuffer()
			Buffer();

			Buffer(const Buffer&) = delete;
			Buffer(Buffer&& other) noexcept :
				rendererID(std::move(other.rendererID)) { 
				other.rendererID = 0;
			}
			Buffer& operator=(const Buffer&) = delete;
			Buffer& operator=(Buffer&& other) noexcept {
				this->rendererID = std::move(other.rendererID);
				other.rendererID = 0;
				return *this;
			}

			~Buffer();

			// Will set the buffer data to data at offset with byte size of size
			void SetBufferData(int64_t size, const void* data = nullptr, uint32_t offset = 0);
			// Will create new data store that can be resized with the same function and fills it with data,
			// calling this will empty out any data previously contained within the buffer.
			// Incompatible with CreateImmutableBuffer
			void CreateMutableBuffer(int64_t size, const void* data = nullptr, BufferDataFlags flags = BufferDataFlags::STATIC_READ);
			// Will create new data store that cannot be changed and fills it with data,
			// this function cannot be called twice.
			// Incompatible with CreateMutableBuffer
			void CreateImmutableBuffer(int64_t size, const void* data = nullptr, BufferStorageFlags flags = BufferStorageFlags::MAP_READ_BIT & BufferStorageFlags::MAP_WRITE_BIT);

			void BindBufferBase(BufferBaseTarget target, uint32_t bindingIndex);

			uint32_t GetRendererID() const {
				return rendererID;
			}

		};


	}

}
