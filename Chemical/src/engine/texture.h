#pragma once

#include <string>

class Texture {

	unsigned int id = 0;


public:
	
	Texture(const std::string& texturePath);
	//void deleteTexture(); currently not managing opengl objects

	void bind(unsigned int slot) const;

};
