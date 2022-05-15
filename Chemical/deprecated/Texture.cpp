#include "Texture.h"

#include <glad/glad.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

Texture::Texture(const std::string& texturePath) {
	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &ID);

	if (data) {
		glTextureStorage2D(ID, 1, GL_RGBA8, width, height);

		if (nrChannels == 3) {
			glTextureSubImage2D(ID, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
		}
		else {
			glTextureSubImage2D(ID, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
		}


	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glGenerateTextureMipmap(ID);
}

void Texture::Bind(unsigned int slot) const {
	glBindTextureUnit(slot, ID);
}