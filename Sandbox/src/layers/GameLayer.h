#pragma once

#include <Chemical.h>
#include <vendor/imgui/imgui.h>

class GameLayer : public Chemical::Core::Layer {

	Chemical::Camera camera;

	Chemical::Mesh* mesh;
	Chemical::Shader* shader;
	Chemical::Transform transform;

	void OnAttach() override {
		mesh = Chemical::Core::AssetImporter::GetMesh("res/meshes/DefaultMesh.mesh");
		shader = Chemical::Core::AssetImporter::GetShader("res/shaders/UnlitShader/UnlitShader.v");

		shader->Bind();

		shader->SetUniformMatrix4FV("v_model", transform.ToMat4());
		shader->SetUniformMatrix4FV("v_proj", camera.proj);
		shader->SetUniformMatrix4FV("v_view", glm::inverse(camera.transform.ToMat4()));

		shader->SetUniform3FV("colour", glm::vec3(0.8f, 0.3f, 0.5f));
	}
	void OnDetach() override {
	}
	void OnUpdate() override {
		camera.Update();





		mesh->Draw();


	}

	~GameLayer() override {

	}
};
