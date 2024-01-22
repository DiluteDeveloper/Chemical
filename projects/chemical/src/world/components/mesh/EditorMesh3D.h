#pragma once

#include "rendering/Vertex3D.h"

#include <vector>

namespace Chemical {
	namespace World {

		// Editor means that this mesh will only be used in the editor, not in scene playback.
		// When this mesh gets converted into a playback mesh, it can either be compressed into
		// drawing data as a Static mesh used for playback, or can have the same information
		// and be dynamic and can be edited during runtime.
		class EditorMesh3D {
		public:
			EditorMesh3D(const std::vector<Rendering::Vertex3D>& vertices, const std::vector<unsigned int>& indices)
				: vertices(vertices), indices(indices) {}

			std::vector<Rendering::Vertex3D> vertices;
			std::vector<unsigned int> indices;
		};
	}
}
