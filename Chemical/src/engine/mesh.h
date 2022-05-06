#pragma once

#include <vector>

class Mesh {

	unsigned int indices = 0;
	unsigned int vao = 0;
	unsigned int buffers[2] = { 0,0 };


public:

	void draw() const;

	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	//void deleteMesh(); temporarily not caring about opengl object destruction bc annoying 
	
	

};
