#pragma once

#include "core/application.h"
#include "scene/scene.h"

namespace Chemical {
	namespace Core {

		class SceneLayer : public ApplicationLayer {
		public:

			// only valid after initialization
			std::unique_ptr<Scene> scene;

		private:

			void InitializeLayer(ApplicationData& appdata) override {
				scene = std::make_unique<Scene>(appdata.GetGLFWWrapper(), appdata.GetDispatcher());
			}

			void UpdateLayer() override {
				scene->Update();

			}


		};
	}
}