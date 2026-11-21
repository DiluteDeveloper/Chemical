#include "block.h"
#include "physics.h"

#include <entt/entity/registry.hpp>

namespace Chemical {
    
    const std::array<uint8_t, 2>& FaceOrientationToVaryingDimensionsIdx(FaceOrientation orientation) {
        static constexpr std::array<std::array<uint8_t, 2>, 6> face_orientation_to_position_dimension_idx = {{
            {0, 2}, // FaceOrientation::Top
            {2, 0}, // FaceOrientation::Bottom
            {2, 1}, // FaceOrientation::Right
            {1, 2}, // FaceOrientation::Left
            {0, 1}, // FaceOrientation::Back
            {1, 0} // FaceOrientation::Front
        }};
        return face_orientation_to_position_dimension_idx[(uint8_t)orientation];
    }

    void BlockData::on_construct(entt::registry& registry, const entt::entity e) {
        BlockData& bd = registry.get<BlockData>(e);
        if(bd.end_position.x < bd.start_position.x || bd.end_position.y < bd.start_position.y || bd.end_position.z < bd.start_position.z)
            spdlog::warn("Block with entity ID {} will not be rendered properly because the start position is less than the end position.", entt::to_integral(e));

        registry.emplace<BoxCollider>(e, (glm::fvec3)bd.start_position, (glm::fvec3)bd.end_position);
    }
    void BlockData::on_update(entt::registry& registry, const entt::entity e) {
        BlockData& bd = registry.get<BlockData>(e);
        // auto swap = [](int32_t& a, int32_t& b) {
        //     int32_t v = a;
        //     a = b;
        //     b = v;
        // };
        if(bd.end_position.x < bd.start_position.x || bd.end_position.y < bd.start_position.y || bd.end_position.z < bd.start_position.z)
            spdlog::warn("Block with entity ID {} will not be rendered properly because the start position is less than the end position.", entt::to_integral(e));
        //     swap(bd.end_position.x, bd.start_position.x);
        // if(bd.end_position.y < bd.start_position.y)
        //     swap(bd.end_position.y, bd.start_position.y);
        // if(bd.end_position.z < bd.start_position.z)
        //     swap(bd.end_position.z, bd.start_position.z);

        BoxCollider& bc = registry.get<BoxCollider>(e);
        bc.min = bd.start_position;
        bc.max = bd.end_position;
    }

    void BlockData::AddTestDataToRegistry(entt::registry& registry) {
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 0, 0), glm::ivec3(5, 0, 5), FaceOrientation::Bottom, BlockType::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 7, 0), glm::ivec3(5, 7, 5), FaceOrientation::Top, BlockType::Grass);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 0, 0), glm::ivec3(0, 3, 5), FaceOrientation::Left, BlockType::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 3, 0), glm::ivec3(0, 4, 5), FaceOrientation::Left, BlockType::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 4, 0), glm::ivec3(0, 6, 5), FaceOrientation::Left, BlockType::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 6, 0), glm::ivec3(0, 7, 5), FaceOrientation::Left, BlockType::GrassDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 0, 0), glm::ivec3(5, 3, 5), FaceOrientation::Right, BlockType::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 3, 0), glm::ivec3(5, 4, 5), FaceOrientation::Right, BlockType::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 4, 0), glm::ivec3(5, 6, 5), FaceOrientation::Right, BlockType::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 6, 0), glm::ivec3(5, 7, 5), FaceOrientation::Right, BlockType::GrassDirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 0, 5), glm::ivec3(5, 3, 5), FaceOrientation::Front, BlockType::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 3, 5), glm::ivec3(5, 4, 5), FaceOrientation::Front, BlockType::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 4, 5), glm::ivec3(5, 6, 5), FaceOrientation::Front, BlockType::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 6, 5), glm::ivec3(5, 7, 5), FaceOrientation::Front, BlockType::GrassDirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 0, 0), glm::ivec3(5, 3, 0), FaceOrientation::Back, BlockType::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 3, 0), glm::ivec3(5, 4, 0), FaceOrientation::Back, BlockType::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 4, 0), glm::ivec3(5, 6, 0), FaceOrientation::Back, BlockType::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 6, 0), glm::ivec3(5, 7, 0), FaceOrientation::Back, BlockType::GrassDirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 0+2, 0+2), glm::ivec3(5+8, 0+2, 5+2), FaceOrientation::Bottom, BlockType::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 7+2, 0+2), glm::ivec3(5+8, 7+2, 5+2), FaceOrientation::Top, BlockType::Grass);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 0+2, 0+2), glm::ivec3(0+8, 3+2, 5+2), FaceOrientation::Left, BlockType::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 3+2, 0+2), glm::ivec3(0+8, 4+2, 5+2), FaceOrientation::Left, BlockType::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 4+2, 0+2), glm::ivec3(0+8, 6+2, 5+2), FaceOrientation::Left, BlockType::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 6+2, 0+2), glm::ivec3(0+8, 7+2, 5+2), FaceOrientation::Left, BlockType::GrassDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5+8, 0+2, 0+2), glm::ivec3(5+8, 3+2, 5+2), FaceOrientation::Right, BlockType::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5+8, 3+2, 0+2), glm::ivec3(5+8, 4+2, 5+2), FaceOrientation::Right, BlockType::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5+8, 4+2, 0+2), glm::ivec3(5+8, 6+2, 5+2), FaceOrientation::Right, BlockType::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5+8, 6+2, 0+2), glm::ivec3(5+8, 7+2, 5+2), FaceOrientation::Right, BlockType::GrassDirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 0+2, 5+2), glm::ivec3(5+8, 3+2, 5+2), FaceOrientation::Front, BlockType::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 3+2, 5+2), glm::ivec3(5+8, 4+2, 5+2), FaceOrientation::Front, BlockType::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 4+2, 5+2), glm::ivec3(5+8, 6+2, 5+2), FaceOrientation::Front, BlockType::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 6+2, 5+2), glm::ivec3(5+8, 7+2, 5+2), FaceOrientation::Front, BlockType::GrassDirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 0+2, 0+2), glm::ivec3(5+8, 3+2, 0+2), FaceOrientation::Back, BlockType::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 3+2, 0+2), glm::ivec3(5+8, 4+2, 0+2), FaceOrientation::Back, BlockType::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 4+2, 0+2), glm::ivec3(5+8, 6+2, 0+2), FaceOrientation::Back, BlockType::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 6+2, 0+2), glm::ivec3(5+8, 7+2, 0+2), FaceOrientation::Back, BlockType::GrassDirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(-40, 4, -40), glm::ivec3(40, 4, 40), FaceOrientation::Top, BlockType::Dirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 5, 5), glm::ivec3(5, 20, 20), FaceOrientation::Left, BlockType::Glass);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(20, 5, 5), glm::ivec3(20, 20, 20), FaceOrientation::Right, BlockType::Glass);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 5, 20), glm::ivec3(20, 20, 20), FaceOrientation::Front, BlockType::Glass);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 5, 5), glm::ivec3(20, 20, 5), FaceOrientation::Back, BlockType::Glass);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 20, 5), glm::ivec3(20, 20, 20), FaceOrientation::Top, BlockType::Glass);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 5, 5), glm::ivec3(20, 5, 20), FaceOrientation::Bottom, BlockType::Glass);
    }
}

