#pragma once

namespace Util {

	// warning: does not copy, aka do not delete the data passed in
	class TypelessArray {
		void* m_data = nullptr;
		uint64_t m_type_size = 0;
		uint64_t m_len = 0;



	public:
		void* data() const { if (m_data != nullptr) return m_data; else LOGGER_CONSOLE_WARNING("TypelessArray data is invalid."); return nullptr; }
		uint64_t type_size() const { return m_type_size; }
		uint64_t len() const { return m_len; }
		uint64_t arr_data_size() const { return (m_type_size * m_len); }

		TypelessArray(void* data, uint64_t type_size, uint64_t len) :
			m_data(data), m_type_size(type_size), m_len(len) {}

		template<typename T>
		TypelessArray(std::vector<T>& data) :
			m_data(static_cast<void*>(data.data())), m_type_size(sizeof(T)), m_len(data.size()) {}

		TypelessArray() = default;

		template<typename T>
		void set(std::vector<T>& data) {
			m_data = static_cast<void*>(data.data());
			m_type_size = sizeof(T);
			m_len = data.size();
		}

		void set(void* data, uint64_t type_size, uint64_t len) {
			m_data = data;
			m_type_size = type_size;
			m_len = len;
		}
	};
}
