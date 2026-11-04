#pragma once

#include <utility>
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

#include "opengl/shader_program.h"
#include "opengl/texture.h"
#include "block_face_group.h"

namespace Chemical {

    namespace Graphics {

        class Renderer {
        public:
            static std::unique_ptr<Renderer> CreateRenderer();

            ~Renderer();

            void Render();
            void UploadBlockFaceGroups(const std::vector<BlockFaceGroup>& groups);

            void UpdateViewMatrix(const glm::mat4& matrix);
            void UpdateProjectionMatrix(const glm::mat4& matrix);

            Renderer(const Renderer& other) = delete;
            Renderer(Renderer&& other) = delete;
            Renderer& operator=(const Renderer&) = delete;
            Renderer& operator=(Renderer&&) = delete;
        private:
            Renderer();

            struct BlockTypeRenderGroup {
                uint32_t vao;
                uint32_t num_groups;
            };
            struct RenderBlockFaceGroup {
                glm::ivec3 min_pos;
                uint32_t block_data_bitmap;
                // Pretty sure this is not padded
            };
            std::vector<BlockTypeRenderGroup> block_type_render_groups;

            std::vector<OpenGL::Texture> textures;
            std::unique_ptr<OpenGL::ShaderProgram> shader;

            // Returns true on success, false on failure
            static bool ConvertBlockFaceGroupsToBlockTypeRenderGroups_Inplace(const std::vector<BlockFaceGroup>& groups, std::vector<BlockTypeRenderGroup>& out);
            static void ConvertBlockFaceGroupsToRenderBlockFaceGroups_Inplace(const std::vector<BlockFaceGroup>& groups, std::vector<RenderBlockFaceGroup>& render_groups);
        };
    }
}
