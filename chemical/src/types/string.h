#pragma once

#define DEFAULT_STRING_ALLOCATION 20

typedef struct {
	size_t len; // null terminator excluded from length
	size_t capacity;
	char* data; // null terminator allocated and included
} string;

// CREATION

// Creates and initializes a string object
extern string string_create();

// FUNCTION

// Turns a string into a const char array
extern char* string_c_str(const string* value);

// Appends a const char* to a string
extern void string_append_c(const char* from, string* to);

// Appends a string to a string
extern void string_append_s(const string* from, string* to);

// DELETION

// Call this at the end of a string's lifetime to delete it.
extern void string_delete(string* value);