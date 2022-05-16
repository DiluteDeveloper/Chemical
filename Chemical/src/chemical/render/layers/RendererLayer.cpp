#include "RendererLayer.h"
#include "chemical/io/ConsoleStream.h"

#include <vendor/glm/gtc/matrix_transform.hpp>

namespace Chemical {

	namespace Render {

		void RendererLayer::OnUpdate() {

			try {
				Scene& s = ai->GetScene("res/scenes/main.scene");

				for (auto& v : s.renderables)
				{
					v.first->Bind();
					v.first->SetUniformMatrix4FV("v_view", glm::mat4(1.0f));
					v.first->SetUniformMatrix4FV("v_proj", glm::perspective(glm::radians(90.0f), 1280.0f / 720.0f, 0.1f, 100.0f));

					for (auto& r : v.second) {
						v.first->SetUniformMatrix4FV("v_model", r.transform.ToMat4());

						r.material.Bind(*v.first);
						r.mesh->Draw();
					}
				}

			}
			catch (std::exception& e) {
				IO::LogError("Chemical::Render::RendererLayer::OnUpdate() \"res/scenes/main.scene\" does not exist.");
				throw e;
			}


		}
	}
}