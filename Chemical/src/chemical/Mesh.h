#pragma once

#include <vector>
#include <string>
#include <filesystem>

namespace Chemical {
	class Mesh {
	public:

		unsigned int indices = 0;
		unsigned int VAO = 0;
		unsigned int buffers[2] = { 0,0 };

		const std::filesystem::path filePath = "";

		void Draw() const;

		Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const std::filesystem::path& filePath);

	};

}
