#include "pch.h"
#include "string.h"

string string_create() {
	string s = { 0, DEFAULT_STRING_ALLOCATION, malloc(DEFAULT_STRING_ALLOCATION) };
	return s;
}

char* string_c_str(const string* value) {
	return value->data;
}

void string_append_c(const char* from, string* to) {
	// Get length of from

	size_t it = strlen(from);

	size_t new_len = to->len + it;
	// check if need to allocate
	if (new_len >= to->capacity) {
		size_t new_alloc_size = max((to->len * 2), (new_len + 1));
		to->data = (char*)realloc(to->data, new_alloc_size); // +1 account for null terminator
		to->capacity = new_len + DEFAULT_STRING_ALLOCATION;
	}
	memcpy_s(to->data + to->len, it + 1, from, it + 1); // +1 account for null terminator
	to->len = new_len;
}

void string_append_s(const string* from, string* to) {

	size_t new_len = to->len + from->len;

	if (new_len >= to->capacity) {
		size_t new_alloc_size = max((to->len * 2), (new_len + 1));
		to->data = (char*)realloc(to->data, new_alloc_size);
		to->capacity = new_len + DEFAULT_STRING_ALLOCATION;
	}
	memcpy_s(to->data + to->len, from->len + 1, from->data, from->len + 1);
	to->len = new_len;
}

void string_delete(string* value) {
	free(value->data);
}