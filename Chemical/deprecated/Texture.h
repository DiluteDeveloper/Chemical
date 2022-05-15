#pragma once

#include <string>

class Texture {

	unsigned int ID = 0;


public:
	
	Texture(const std::string& texturePath);
	//void deleteTexture(); currently not managing opengl objects

	void Bind(unsigned int slot) const;

};
