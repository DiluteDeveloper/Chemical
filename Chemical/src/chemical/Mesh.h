#pragma once

#include <vector>
#include <string>

namespace Chemical {
	class Mesh {
	public:

		unsigned int indices = 0;
		unsigned int VAO = 0;
		unsigned int buffers[2] = { 0,0 };

		std::string filePath = "";

		void Draw() const;

		Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::string& filePath);

	};

}
