#pragma once

#include "graphics/opengl/vertex_array.h"
#include "graphics/opengl/texture.h"
#include "util/image_loader.h"

/*
The data that mesh has to store is encoded in the name.
This is so we dont have to store data in the mesh that
is fairly trivial, such as the drawmode, whether it uses
array buffers or element buffers, or the element buffer
type.

encoding:

class Mesh_x_y_z_w

x = DrawMode

y = element buffer data type

z = is dynamic

w = is instanced

x encoding:

pnt = DrawMode::POINTS
lst = DrawMode::LINE_STRIP
llo = DrawMode::LINE_LOOP
lin = DrawMode::LINES
lsa = DrawMode::LINE_STRIP_ADJACENCY
laj = DrawMode::LINES_ADJACENCY
tst = DrawMode::TRIANGLE_STRIP
tfn = DrawMode::TRIANGLE_FAN
tri = DrawMode::TRIANGLES
tsa = DrawMode::TRIANGLE_STRIP_ADJACENCY
taj = DrawMode::TRIANGLES_ADJACENCY
pch = DrawMode::PATCHES


y:

nil = uses array buffer drawing
ub = UNSIGNED_BYTE
ui = UNSIGNED_INT
us = UNSIGNED_SHORT

z:

s = static(immutable)
d = dynamic(mutable)

w:

y = is instanced
n = is not instanced

// might need more parameters later

*/

namespace Default3D {
	class Material {
		friend class Renderer;

		std::unique_ptr<OpenGL::Texture> diffTexture;
		std::unique_ptr<OpenGL::Texture> specMap;

		static std::unique_ptr<OpenGL::Texture> CreateTexture(std::string_view texturePath, bool mipmap) {



			std::unique_ptr<OpenGL::Texture> texture;

			Util::ImageData iData = Util::LoadImage(texturePath, 3);
			{
				OpenGL::TextureStorageParameters sp;

				sp.width = iData.x;
				sp.height = iData.y;
				sp.internalFormat = OpenGL::TextureInternalFormat::RGB8;

				if (mipmap)
					sp.mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(sp.width, sp.height)))) + 1; // copied straight from the net
				sp.textureType = OpenGL::TextureType::TEXTURE_2D;

				texture = std::make_unique<OpenGL::Texture>(sp);
			}

			{
				OpenGL::TextureDataParameters dp;

				dp.baseFormat = OpenGL::TextureBaseFormat::RGB;
				dp.dataType = OpenGL::DataType::UNSIGNED_BYTE;
				dp.height = iData.y;
				dp.width = iData.x;
				dp.textureType = OpenGL::TextureType::TEXTURE_2D;

				texture->SetTextureData(dp, iData.data);
				if (mipmap)
					texture->GenerateMipmaps();
			}
			return texture;
		}

	public:
		glm::vec3 diffColour = glm::vec3(1);

		glm::vec3 specColour = glm::vec3(1);

		Material(std::string_view diffTexturePath, std::string_view specMapPath, glm::vec3 diffColour = glm::vec3(1), glm::vec3 specColour = glm::vec3(1)) :
			diffColour(diffColour), specColour(specColour), diffTexture(CreateTexture(diffTexturePath, true)), specMap(CreateTexture(specMapPath, false)) { }
	};

	struct Vertex {
		glm::fvec3 position;
		glm::fvec2 texCoord;

		Vertex(glm::fvec3 position, glm::fvec2 texCoord) :
			position(position), texCoord(texCoord) {}
	};

	// This class accounts for the most common use case of mesh. 
	// Mesh_tri_ui_s_n
	class Mesh_tri_ui_s_n {
		OpenGL::VertexArray vertexArray;
		uint32_t count;

	public:

		Mesh_tri_ui_s_n(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
			OpenGL::Buffer vertexBuffer;
			vertexBuffer.CreateImmutableBuffer((uint32_t)(sizeof(Vertex) * vertices.size()), vertices.data(), OpenGL::BufferStorageFlags::MAP_READ_BIT);
			OpenGL::Buffer elementBuffer;
			elementBuffer.CreateImmutableBuffer((uint32_t)(sizeof(uint32_t) * indices.size()), indices.data(), OpenGL::BufferStorageFlags::MAP_READ_BIT);

			OpenGL::VertexBufferInfo info(sizeof(Vertex), 0, 0);
			info.AddAttribute(OpenGL::VertexAttribute{ 3, 0 });
			info.AddAttribute(OpenGL::VertexAttribute{ 2, offsetof(Vertex, texCoord) });

			vertexArray.SetVertexBuffer(vertexBuffer, info);
			vertexArray.SetElementBuffer(elementBuffer);

			count = (uint32_t)indices.size();
		}

		void Draw() const {
			OpenGL::ElementDrawInfo edi;
			edi.count = count;
			edi.dataType = OpenGL::DataType::UNSIGNED_INT;
			edi.mode = OpenGL::DrawMode::TRIANGLES;
			edi.offset = 0;
			vertexArray.Bind();
			vertexArray.DrawElements(edi);
		}
	};
}

