#pragma once

#include <entt/entt.h>

#include "components/mesh/EditorMesh3DHandler.h"

#include "components/transform/Transform3D.h"

#include "graphics/opengl/Handler.h"
#include "rendering/Renderer3D.h"

namespace Chemical {
	namespace World {
		class Scene {

			EditorMesh3DHandler meshHandler;


		public:

			// might give scene engine if needed
			Scene(OpenGL::Handler* handler, Rendering::Renderer3D* renderer)
				: meshHandler(handler, renderer) { 
			
			}

			void LoadScene() {
				meshHandler.UploadMeshes(r);
			}

			entt::entity CreateEntity() {
				entt::entity e = r.create();

				// Transform3D comes default
				r.emplace<Transform3D>(e);

				return e;
			}

			//hardcoding this for now

			template<typename component, typename... arguments>
			void CreateComponent(entt::entity e, arguments... args) {
				r.emplace<component>(e, args...);
			}

		private:
			entt::registry r;
		};
	}
}