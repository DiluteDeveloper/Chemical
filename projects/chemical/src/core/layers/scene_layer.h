#pragma once

#include "core/application.h"

#include "scene_objects/object.h"
#include "scene_objects/camera_3d.h"

namespace Chemical {
	namespace Core {

		// currently does not manage scene lifetime itself;
		class SceneLayer : public ApplicationLayer {
		public:

			void SetRoot(Scene::Object* root) {
				m_root = root;
			}
			Scene::Object* GetRoot() const {
				return m_root;
			}
			void SetCamera3D(Scene::Camera3D* camera_3d) {
				m_camera_3d = camera_3d;
			}
			Scene::Camera3D* GetCamera3D() {
				return m_camera_3d;
			}
		private:
			using ApplicationLayer::ApplicationLayer;
			friend ApplicationData;

			void UpdateLayer() override {
				if (m_camera_3d != nullptr)
					m_camera_3d->UpdateMovement();

				
			}

			Scene::Object* m_root = nullptr;
			Scene::Camera3D* m_camera_3d = nullptr;
		};
	}
}