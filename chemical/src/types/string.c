#include "pch.h"
#include "string.h"

string string_create() {
	string s = { 0, (string_data*)malloc(sizeof(string_data)) };
	s.data->capacity = DEFAULT_STRING_ALLOCATION;
	s.data->data = malloc(DEFAULT_STRING_ALLOCATION);
	return s;
}

string string_create_value(const char* value) {
	size_t it = 0;
	while (value[it] != '\0') it++;
	it++;

	string s = { it, (string_data*)malloc(sizeof(string_data)) };
	s.data->capacity = s.len + DEFAULT_STRING_ALLOCATION;
	s.data->data = malloc(s.data->capacity);
	memcpy_s(s.data->data, it, (void*)value, it);
	return s;
}

char* string_c_str(const string* value) {
	return value->data->data;
}

void string_append_c(const char* from, string* to) {
	// Get length of from

	size_t it = 0;
	while (from[it] != '\0') it++;
	it++;

	// allocate if needed

	// check if need to allocate
	if (to->len + it >= to->data->capacity) {
		to->data->capacity = to->len + it - 1 + DEFAULT_STRING_ALLOCATION;
		char* new = malloc(to->data->capacity);
		memcpy_s(new, to->len, to->data->data, to->len);
		free(to->data->data);

		size_t new_data_offset = 0;
		// new allocation
		if (to->len > 0) // if string has null terminator, replace it
			new_data_offset = to->len - 1;
		else
			new_data_offset = to->len;

		memcpy_s(new + new_data_offset, it, from, it);
		to->len = new_data_offset + it;

		to->data->data = new;
	}
	else {

		size_t new_data_offset = 0;

		if (to->len > 0) // if string has null terminator, replace it
			new_data_offset = to->len - 1;
		else
			new_data_offset = to->len;

		memcpy_s(to->data->data + new_data_offset, it, from, it);
		to->len = new_data_offset + it;
	}
}

void string_append_s(const string* from, string* to) {
	// not currently implemented
}

void string_delete(string* value) {
	free(value->data->data);
	free(value->data);
}