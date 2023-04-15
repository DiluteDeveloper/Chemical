#pragma once

#define DEFAULT_STRING_ALLOCATION 20

typedef struct {
	size_t capacity;
	char* data;
} string_data;

typedef struct {
	size_t len;
	string_data* data;
} string;

// CREATION

// Creates and initializes a string object
string string_create();

// FUNCTION

// Turns a string into a const char array
char* string_c_str(const string* value);

// Appends a const char* to a string
void string_append_c(const char* from, string* to);

// Appends a string to a string
void string_append_s(const string* from, string* to);

// DELETION

// Call this at the end of a string's lifetime to delete it.
void string_delete(string* value);