#pragma once

#include <vector>
#include <string>
#include <filesystem>

namespace Chemical {

	namespace Render {
		struct Mesh {

			const unsigned int indices = 0;
			unsigned int VAO = 0;
			unsigned int buffers[2] = { 0,0 };

			void Draw() const;

			Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
			~Mesh();// will cause RAII issues, cant b fucked

			//Mesh(const Mesh&) = delete;
			//Mesh(Mesh&&) = delete;
		};
	}


}
