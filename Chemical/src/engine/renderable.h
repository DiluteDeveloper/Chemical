#pragma once

#include "transform.h"
#include "mesh.h"
#include "material.h"

struct Renderable {

	Transform transform;
	Material material;
	Mesh mesh;

	Renderable(const Mesh& mesh) : mesh(mesh) {};

};