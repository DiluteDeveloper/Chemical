#pragma once

#include "Layer.h"

#include <vendor/imgui/imgui.h>
#include <unordered_map>
#include <filesystem>

namespace Chemical {
	namespace Layers {
		class GUILayer : public Layer {

			const std::unordered_map<std::string, std::vector<std::filesystem::path>>& resourceStructure;

			std::filesystem::path currentDirectory;

			void OnUpdate() override {
				ImGui::Begin("Resources");


				for (const auto vp : resourceStructure.at(currentDirectory.generic_string())) {
					if (std::filesystem::is_directory(vp)) {
						if (ImGui::Button(vp.filename().generic_string().c_str())) {
							currentDirectory = vp;
						}
					}
					else
						ImGui::Text(vp.filename().generic_string().c_str());

				}

				if (currentDirectory != "res")
					if (ImGui::Button("Back")) {
						currentDirectory = currentDirectory.parent_path(); // ugly code
					}
				ImGui::End();
			}
		public:
			GUILayer(const std::unordered_map<std::string, std::vector<std::filesystem::path>>& resourceStructure) : resourceStructure(resourceStructure) {
				currentDirectory = "res";
			}


			~GUILayer() override {

			}
		};

	}
}
