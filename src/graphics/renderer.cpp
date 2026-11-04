#include <GLFW/glfw3.h> 
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#include "util/filestream.h"
#include "gl.h"
#include "renderer.h"

namespace Chemical {
namespace Graphics {

#if defined(GLAD)
    // OpenGL debug callback
    void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param)
    {

        // prevents message duplication
        int64_t prevMessageID = -1;

        if ((GLuint)prevMessageID == id)
            return;
        prevMessageID = id;
        auto const src_str = [source]() {
            switch (source)
            {
            case GL_DEBUG_SOURCE_API: return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "WINDOW SYSTEM";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
            case GL_DEBUG_SOURCE_THIRD_PARTY: return "THIRD PARTY";
            case GL_DEBUG_SOURCE_APPLICATION: return "APPLICATION";
            case GL_DEBUG_SOURCE_OTHER: return "OTHER";
            }
            return "SOURCE";
        }();


        auto const type_str = [type]() {
            switch (type)
            {
            case GL_DEBUG_TYPE_ERROR: return "ERROR";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED_BEHAVIOR";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED_BEHAVIOR";
            case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
            case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
            case GL_DEBUG_TYPE_MARKER: return "MARKER";
            case GL_DEBUG_TYPE_OTHER: return "OTHER";
            default: break;
            }
            return "TYPE";
        }();

        switch (severity) {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            spdlog::info("{}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            spdlog::warn("{}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            spdlog::warn("{}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            spdlog::error("{}, {}, {}: {}", src_str, type_str, (int)id, (char*)message);
            break;
        default: break;
        }

    }

#endif

    std::unique_ptr<Renderer> Renderer::CreateRenderer() {
        if(glfwGetCurrentContext() == nullptr) {
            spdlog::error("Renderer cannot be created because there is no OpenGL context attached!");
            return nullptr;
        }            
        // Renderer* renderer = new Renderer();
        return std::unique_ptr<Renderer>(new Renderer());
    }

    Renderer::Renderer() {

#if defined(GLAD)
        glDebugMessageCallback(&MessageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

        // Each texture is moved into core->textures after it is set up. The GL texture name moves with it
        // (the moved-from object is left with id 0 and deletes nothing), so the unit bindings stay valid.
        textures.reserve(7);

        int width=0, height=0, channels=0;
        void* dirtdata = stbi_load("res/textures/blocks/dirt.png", &width, &height, &channels, 0);

        OpenGL::TextureStorageParameters p;
        p.internalFormat = OpenGL::TextureInternalFormat::RGB8;
        p.width = width;
        p.height = height;
        OpenGL::Texture dirt(p);

        OpenGL::TextureDataParameters param;
        param.baseFormat = OpenGL::TextureBaseFormat::RGB;
        param.dataType = OpenGL::DataType::UNSIGNED_BYTE;
        param.width = width;
        param.height = height;
        // param.textureType = OpenGL::TextureType::TEXTURE_2D;
        dirt.SetTextureData(param, dirtdata);

        stbi_image_free(dirtdata);

        dirt.GenerateMipmaps();

        dirt.BindTexture(1);

        dirt.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        dirt.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(dirt));


        void* grassdata = stbi_load("res/textures/blocks/grass.png", &width, &height, &channels, 0);

        OpenGL::Texture grass(p);

        grass.SetTextureData(param, grassdata);

        stbi_image_free(grassdata);

        grass.GenerateMipmaps();

        grass.BindTexture(2);

        grass.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        grass.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(grass));

        void* stonedata = stbi_load("res/textures/blocks/stone.png", &width, &height, &channels, 0);

        OpenGL::Texture stone(p);

        stone.SetTextureData(param, stonedata);

        stbi_image_free(stonedata);

        stone.GenerateMipmaps();

        stone.BindTexture(0);

        stone.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        stone.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(stone));

        void* beddata = stbi_load("res/textures/blocks/bedrock.png", &width, &height, &channels, 0);

        OpenGL::Texture bed(p);

        bed.SetTextureData(param, beddata);

        stbi_image_free(beddata);

        bed.GenerateMipmaps();

        bed.BindTexture(3);

        bed.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        bed.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(bed));

        void* logtopdata = stbi_load("res/textures/blocks/oaklogtop.png", &width, &height, &channels, 0);

        OpenGL::Texture logtop(p);

        logtop.SetTextureData(param, logtopdata);

        stbi_image_free(logtopdata);

        logtop.GenerateMipmaps();

        logtop.BindTexture(4);

        logtop.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        logtop.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(logtop));

        void* logsidedata = stbi_load("res/textures/blocks/oaklogside.png", &width, &height, &channels, 0);

        OpenGL::Texture logside(p);

        logside.SetTextureData(param, logsidedata);

        stbi_image_free(logsidedata);

        logside.GenerateMipmaps();

        logside.BindTexture(5);

        logside.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        logside.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(logside));

        OpenGL::TextureStorageParameters pleaves;
        pleaves.internalFormat = OpenGL::TextureInternalFormat::RGBA8;
        pleaves.width = width;
        pleaves.height = height;

        OpenGL::TextureDataParameters paramleaves;
        paramleaves.baseFormat = OpenGL::TextureBaseFormat::RGBA;
        paramleaves.dataType = OpenGL::DataType::UNSIGNED_BYTE;
        paramleaves.width = width;
        paramleaves.height = height;
        // paramleaves.textureType = OpenGL::TextureType::TEXTURE_2D;

        void* leavesdata = stbi_load("res/textures/blocks/leaves.png", &width, &height, &channels, 0);

        OpenGL::Texture leaves(pleaves);

        leaves.SetTextureData(paramleaves, leavesdata);

        stbi_image_free(leavesdata);

        leaves.GenerateMipmaps();

        leaves.BindTexture(6);

        leaves.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MIN_FILTER, GL_NEAREST);
        leaves.SetTextureSetting(OpenGL::TextureSettings::TEXTURE_MAG_FILTER, GL_NEAREST);

        textures.push_back(std::move(leaves));

        textures[2].BindTexture(0);
        textures[0].BindTexture(1);
        textures[1].BindTexture(2);
        textures[3].BindTexture(3);

        // loader = std::make_unique<ChunkLoader>(glm::vec3{}, 300, 8);

        // The shader objects can stay local: once the program is linked it doesn't need them any more.
        OpenGL::Shader vertex_chunk_shader(Util::ReadFile("res/shaders/block_shader.vert").c_str(), OpenGL::ShaderType::VERTEX_SHADER);
        OpenGL::Shader fragment_chunk_shader(Util::ReadFile("res/shaders/block_shader.frag").c_str(), OpenGL::ShaderType::FRAGMENT_SHADER);


        // OpenGL::VertexLayout vertex_layout;
        // vertex_layout.AddAttribute(OpenGL::VertexAttribute{ 1, 0, OpenGL::DataType::UNSIGNED_INT, OpenGL::DataTransformation::INT });

        shader = std::make_unique<OpenGL::ShaderProgram>(std::initializer_list<
            const OpenGL::Shader*>{ &vertex_chunk_shader, & fragment_chunk_shader });

        shader->SetUniform1I("stoneTexture", 0);
        shader->SetUniform1I("dirtTexture", 1);
        shader->SetUniform1I("grassTexture", 2);
        shader->SetUniform1I("bedrockTexture", 3);
        shader->SetUniform1I("oaklogtopTexture", 4);
        shader->SetUniform1I("oaklogsideTexture", 5);
        shader->SetUniform1I("oakleavesTexture", 6);
    }
    Renderer::~Renderer() {
        for(const BlockTypeRenderGroup& btrg : block_type_render_groups) {
            glDeleteBuffers(1, &btrg.vao);
        }
    }

    void Renderer::Render() {

		shader->BindProgram();
        for(const BlockTypeRenderGroup& bt : block_type_render_groups) {
            glBindVertexArray(bt.vao);
            glDrawArraysInstanced(GL_POINTS, 0, bt.num_groups, 6);
        }
    }
 
    void Renderer::UpdateViewMatrix(const glm::mat4& matrix) {
		shader->BindProgram();
		shader->SetUniformMatrix4FV("view_matrix", 1, false, &matrix[0][0]);
    }
    void Renderer::UpdateProjectionMatrix(const glm::mat4& matrix) {
		shader->BindProgram();
		shader->SetUniformMatrix4FV("projection_matrix", 1, false, &matrix[0][0]);
    }
    void UpdateProjectionMatrix(const glm::mat4& matrix);
    void Renderer::UploadBlockFaceGroups(const std::vector<BlockFaceGroup>& groups) {
        ConvertBlockFaceGroupsToBlockTypeRenderGroups_Inplace(groups, block_type_render_groups);
    }

    void Renderer::ConvertBlockFaceGroupsToRenderBlockFaceGroups_Inplace(const std::vector<BlockFaceGroup>& groups, std::vector<RenderBlockFaceGroup>& render_groups) {

        constexpr int MIN_BITMAP_COMPONENT_VALUE = 0;

        constexpr int BLOCK_TYPE_BITSHIFT = 32 - 8;
        constexpr int MAX_BLOCK_TYPE_VALUE = 255;

        constexpr int SIZE_X_BITSHIFT = BLOCK_TYPE_BITSHIFT - 7;
        constexpr int SIZE_Y_BITSHIFT = SIZE_X_BITSHIFT - 7;
        constexpr int SIZE_Z_BITSHIFT = SIZE_Y_BITSHIFT - 7;
        constexpr int MAX_SIZE_VALUE = 127;

        constexpr int BLOCK_FACE_ORIENTATION_BITSHIFT = SIZE_Z_BITSHIFT - 3;
        constexpr int MAX_BLOCK_FACE_ORIENTATION_VALUE = 5;

        for(const BlockFaceGroup& bfg : groups) {
            
            auto& render_block_face_group = render_groups.emplace_back();

            // Set block type
            
            if((int)bfg.block_type > MAX_BLOCK_TYPE_VALUE || 
                (int)bfg.block_type < MIN_BITMAP_COMPONENT_VALUE) {
                spdlog::warn("Block type ID {} is out of valid range ({}-{})! using block type ID {} instead.", 
                        (int)bfg.block_type, MIN_BITMAP_COMPONENT_VALUE, MAX_BLOCK_TYPE_VALUE, MAX_BLOCK_TYPE_VALUE); 

                render_block_face_group.block_data_bitmap = MAX_BLOCK_TYPE_VALUE << BLOCK_TYPE_BITSHIFT;
            } else 
                render_block_face_group.block_data_bitmap = (int)bfg.block_type << BLOCK_TYPE_BITSHIFT;

            // Calculate minimum x,y,z corner and set size and minimum position
            
            auto SetBlockFaceGroupOffsetAndMinPosition = [&](int start_pos, int end_pos, int bitshift, int& min_pos) {

                int offset;
                if(start_pos < end_pos) {
                    min_pos = start_pos;
                    offset = (end_pos - start_pos);
                } else {
                    min_pos = end_pos;
                    offset = (start_pos - end_pos);
                }
                if((int)offset > MAX_SIZE_VALUE || 
                        (int)offset < MIN_BITMAP_COMPONENT_VALUE) {
                    spdlog::warn("Block group size {} is out of valid range ({}-{})! using a size of {} instead.", 
                            offset, MIN_BITMAP_COMPONENT_VALUE, MAX_SIZE_VALUE, MAX_SIZE_VALUE); 
                    offset = MAX_SIZE_VALUE;
                }
                render_block_face_group.block_data_bitmap += offset << bitshift;
            };

            SetBlockFaceGroupOffsetAndMinPosition(bfg.start_position.x, bfg.end_position.x, SIZE_X_BITSHIFT, render_block_face_group.min_pos.x);
            SetBlockFaceGroupOffsetAndMinPosition(bfg.start_position.y, bfg.end_position.y, SIZE_Y_BITSHIFT, render_block_face_group.min_pos.y);
            SetBlockFaceGroupOffsetAndMinPosition(bfg.start_position.z, bfg.end_position.z, SIZE_Z_BITSHIFT, render_block_face_group.min_pos.z);

            if((int)bfg.block_face_orientation > MAX_BLOCK_FACE_ORIENTATION_VALUE ||
                    (int)bfg.block_face_orientation < MIN_BITMAP_COMPONENT_VALUE) {

                    spdlog::warn("Block face orientation ID {} is invalid! using {} instead.", 
                            (int)bfg.block_face_orientation, MAX_BLOCK_FACE_ORIENTATION_VALUE); 

                    render_block_face_group.block_data_bitmap += 
                        MAX_BLOCK_FACE_ORIENTATION_VALUE << BLOCK_FACE_ORIENTATION_BITSHIFT;
            }
            render_block_face_group.block_data_bitmap += 
                (int)bfg.block_face_orientation << BLOCK_FACE_ORIENTATION_BITSHIFT;

        }
    }
    bool Renderer::ConvertBlockFaceGroupsToBlockTypeRenderGroups_Inplace(const std::vector<BlockFaceGroup>& groups, std::vector<BlockTypeRenderGroup>& out) {

        
        std::vector<RenderBlockFaceGroup> render_block_face_groups;
        ConvertBlockFaceGroupsToRenderBlockFaceGroups_Inplace(groups, render_block_face_groups);

        constexpr int BLOCK_TYPE_BITMASK = 0xFF000000;
        constexpr int BLOCK_TYPE_BITSHIFT = 32 - 8;

        std::sort(render_block_face_groups.begin(), render_block_face_groups.end(), 
                [](const RenderBlockFaceGroup& a, const RenderBlockFaceGroup& b) {
            return 
                (a.block_data_bitmap & BLOCK_TYPE_BITMASK) < 
                (b.block_data_bitmap & BLOCK_TYPE_BITMASK);
        });

        int last_block_type = -1;
        uint32_t last_block_type_len = 0;
        std::vector<std::pair<const RenderBlockFaceGroup*, uint32_t>> block_type_buffer_starts;

        for(const RenderBlockFaceGroup& rbfg : render_block_face_groups) {
            spdlog::info("minpos: {}, {}, {}", rbfg.min_pos.x,rbfg.min_pos.y,rbfg.min_pos.z);
            spdlog::info("block type: {}", (rbfg.block_data_bitmap >> 24) & 0xFF);
            // spdlog::info("scale: {}, {}, {}", (rbfg.block_data_bitmap >> 17) & 127,
            //         (rbfg.block_data_bitmap >> 10) & 127,(rbfg.block_data_bitmap >> 3) & 127);


            spdlog::info("blocktype: {} min_pos: {} {} {}", rbfg.block_data_bitmap, rbfg.min_pos.x, rbfg.min_pos.y, rbfg.min_pos.z);
            last_block_type_len++;

            int block_type = rbfg.block_data_bitmap >> BLOCK_TYPE_BITSHIFT;
            if(last_block_type != block_type) {
                block_type_buffer_starts.emplace_back(&rbfg, last_block_type_len);
                last_block_type = block_type;
            }

        }

        for(const auto& pair : block_type_buffer_starts) {
			uint32_t vbo;
            uint32_t& vao = out.emplace_back(0, pair.second).vao; 

			glGenBuffers(1, &vbo);
            glGenVertexArrays(1, &vao);

            glBindVertexArray(vao);

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, pair.second * sizeof(RenderBlockFaceGroup), pair.first, GL_STATIC_DRAW);

            glVertexAttribIPointer(0, 3, GL_INT, sizeof(RenderBlockFaceGroup), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, sizeof(RenderBlockFaceGroup), (void*)(3 * sizeof(int)));
            glEnableVertexAttribArray(1);

            // glBindVertexBuffer(0, vbo, 0, sizeof(RenderBlockFaceGroup));

            // glVertexAttribBinding(0, 0);
            // glVertexAttribBinding(1, 0);

            // glVertexBindingDivisor(0, 6);
            // glVertexAttribDivisor(0, 6);
            // glVertexAttribDivisor(1, 6);

            // glDeleteBuffers(1, &vbo);
        }

        return true;
    }

}
}
