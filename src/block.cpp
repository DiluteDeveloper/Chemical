#include "block.h"

#include <entt/entity/registry.hpp>

namespace Chemical {

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

