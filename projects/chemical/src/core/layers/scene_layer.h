#pragma once

#include "core/application.h"
#include "world/scene.h"

namespace Chemical {
	namespace Core {

		// currently does not manage scene lifetime itself;
		class SceneLayer : protected ApplicationLayer {
		public:

			void SetScene(Scene* f_scene) {
				scene = f_scene;
			}
			const Scene* GetScene() const {
				return scene;
			}
		private:
			using ApplicationLayer::ApplicationLayer;
			friend ApplicationData;

			const Scene* scene = nullptr;
		};
	}
}