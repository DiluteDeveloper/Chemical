#pragma once

namespace ChemicalAPI {
	extern const char* make_charp();

	extern std::string make_string();

	template<typename T>
	uint64_t size() {
		return sizeof(T);
	}
}

