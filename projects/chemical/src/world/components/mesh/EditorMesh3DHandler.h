#pragma once

#include "EditorMesh3D.h"
#include "graphics/opengl/handler.h"
#include "rendering/Renderer3D.h"
#include "rendering/Vertex3D.h"

#include <entt/entt.h>

namespace Chemical {
	namespace World {

		// loads editor meshes
		// possibly make polymorphic with other handlers at some point
		class EditorMesh3DHandler {

		public:
			EditorMesh3DHandler(OpenGL::Handler* handler, Rendering::Renderer3D* renderer) :
				handler(handler), renderer(renderer) {}

			// uploading static meshes on scene load
			// eventually want to NOT give the entire registry to this function
			void UploadMeshes(const entt::registry& r) {

				auto view = r.view<EditorMesh3D>();
				for (auto [e, m] : view.each()) {

					OpenGL::ElementDrawInfo* info = new OpenGL::ElementDrawInfo(m.indices.size()); // VERY TEMPORARY MEMORY LEAK

					Rendering::RawMesh3D* r = renderer->CreateRawMesh(*info);

					handler->SubmitFunctionExecution([r, m, &renderer = renderer] {
						r->vBuffer.CreateImmutableBuffer(m.vertices.size() * sizeof(Rendering::Vertex3D), &m.vertices[0]);
						r->eBuffer.CreateImmutableBuffer(m.indices.size() * sizeof(unsigned int), &m.indices[0]);
						r->vArray.SetVertexBuffer(r->vBuffer, renderer->defLayout, 0, 0);
						r->vArray.SetElementBuffer(r->eBuffer);
						});
				}
			}

		private:
			OpenGL::Handler* handler = nullptr;
			Rendering::Renderer3D* renderer = nullptr;

		};
	}
}