#pragma once

#include <string>

class Texture {

	unsigned int id = 0;


public:
	
	Texture(const std::string& texturePath);
	~Texture();

	void bind(unsigned int slot);

};
