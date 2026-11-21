#include "block.h"

#include <entt/entity/registry.hpp>

namespace Chemical {

    void BlockData::AddTestDataToRegistry(entt::registry& registry) {
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 0, 0), glm::ivec3(5, 0, 5), FaceOrientation::Bottom, Type::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 7, 0), glm::ivec3(5, 7, 5), FaceOrientation::Top, Type::Grass);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 0, 0), glm::ivec3(0, 3, 5), FaceOrientation::Left, Type::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 3, 0), glm::ivec3(0, 4, 5), FaceOrientation::Left, Type::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 4, 0), glm::ivec3(0, 6, 5), FaceOrientation::Left, Type::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 6, 0), glm::ivec3(0, 7, 5), FaceOrientation::Left, Type::GrassDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 0, 0), glm::ivec3(5, 3, 5), FaceOrientation::Right, Type::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 3, 0), glm::ivec3(5, 4, 5), FaceOrientation::Right, Type::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 4, 0), glm::ivec3(5, 6, 5), FaceOrientation::Right, Type::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 6, 0), glm::ivec3(5, 7, 5), FaceOrientation::Right, Type::GrassDirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 0, 5), glm::ivec3(5, 3, 5), FaceOrientation::Front, Type::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 3, 5), glm::ivec3(5, 4, 5), FaceOrientation::Front, Type::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 4, 5), glm::ivec3(5, 6, 5), FaceOrientation::Front, Type::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 6, 5), glm::ivec3(5, 7, 5), FaceOrientation::Front, Type::GrassDirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 0, 0), glm::ivec3(5, 3, 0), FaceOrientation::Back, Type::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 3, 0), glm::ivec3(5, 4, 0), FaceOrientation::Back, Type::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 4, 0), glm::ivec3(5, 6, 0), FaceOrientation::Back, Type::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0, 6, 0), glm::ivec3(5, 7, 0), FaceOrientation::Back, Type::GrassDirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 0+2, 0+2), glm::ivec3(5+8, 0+2, 5+2), FaceOrientation::Bottom, Type::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 7+2, 0+2), glm::ivec3(5+8, 7+2, 5+2), FaceOrientation::Top, Type::Grass);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 0+2, 0+2), glm::ivec3(0+8, 3+2, 5+2), FaceOrientation::Left, Type::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 3+2, 0+2), glm::ivec3(0+8, 4+2, 5+2), FaceOrientation::Left, Type::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 4+2, 0+2), glm::ivec3(0+8, 6+2, 5+2), FaceOrientation::Left, Type::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 6+2, 0+2), glm::ivec3(0+8, 7+2, 5+2), FaceOrientation::Left, Type::GrassDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5+8, 0+2, 0+2), glm::ivec3(5+8, 3+2, 5+2), FaceOrientation::Right, Type::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5+8, 3+2, 0+2), glm::ivec3(5+8, 4+2, 5+2), FaceOrientation::Right, Type::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5+8, 4+2, 0+2), glm::ivec3(5+8, 6+2, 5+2), FaceOrientation::Right, Type::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5+8, 6+2, 0+2), glm::ivec3(5+8, 7+2, 5+2), FaceOrientation::Right, Type::GrassDirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 0+2, 5+2), glm::ivec3(5+8, 3+2, 5+2), FaceOrientation::Front, Type::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 3+2, 5+2), glm::ivec3(5+8, 4+2, 5+2), FaceOrientation::Front, Type::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 4+2, 5+2), glm::ivec3(5+8, 6+2, 5+2), FaceOrientation::Front, Type::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 6+2, 5+2), glm::ivec3(5+8, 7+2, 5+2), FaceOrientation::Front, Type::GrassDirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 0+2, 0+2), glm::ivec3(5+8, 3+2, 0+2), FaceOrientation::Back, Type::Stone);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 3+2, 0+2), glm::ivec3(5+8, 4+2, 0+2), FaceOrientation::Back, Type::StoneDirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 4+2, 0+2), glm::ivec3(5+8, 6+2, 0+2), FaceOrientation::Back, Type::Dirt);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(0+8, 6+2, 0+2), glm::ivec3(5+8, 7+2, 0+2), FaceOrientation::Back, Type::GrassDirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(-40, 4, -40), glm::ivec3(40, 4, 40), FaceOrientation::Top, Type::Dirt);

		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 5, 5), glm::ivec3(5, 20, 20), FaceOrientation::Left, Type::Glass);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(20, 5, 5), glm::ivec3(20, 20, 20), FaceOrientation::Right, Type::Glass);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 5, 20), glm::ivec3(20, 20, 20), FaceOrientation::Front, Type::Glass);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 5, 5), glm::ivec3(20, 20, 5), FaceOrientation::Back, Type::Glass);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 20, 5), glm::ivec3(20, 20, 20), FaceOrientation::Top, Type::Glass);
		registry.emplace<BlockData>(registry.create(), glm::ivec3(5, 5, 5), glm::ivec3(20, 5, 20), FaceOrientation::Bottom, Type::Glass);
    }
}

