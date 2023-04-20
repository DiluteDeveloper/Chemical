#include "pch.h"
#include "renderer.h"

void Renderer::InitializeRenderer() {
	/*Shader vertexShader(Util::ReadFile("resources/shaders/default_shader.vert").c_str(), ShaderType::VERTEX_SHADER);
	Shader fragmentShader(Util::ReadFile("resources/shaders/default_shader.frag").c_str(), ShaderType::FRAGMENT_SHADER);

	shaderProgram = std::make_unique<ShaderProgram>(std::initializer_list<const Shader*>({ &vertexShader, &fragmentShader }));

	shaderProgram->SetUniform1I("diffTexture", 0);
	shaderProgram->SetUniform1I("specMap", 1);*/
}

void Renderer::SceneRenderer(const Scene& scene) {
	/*shaderProgram->BindProgram();


	const auto& view = scene.m_registry.view <Transform, Default3D::Material, Default3D::Mesh_tri_ui_s_n>();


	for (const auto& [entity, transform, mat, mesh] : view.each()) {
		shaderProgram->SetUniformMatrix4FV("v_model", 1, false, &transform.GetTransform()[0][0]); // bit worrysome of a cast

		mat.diffTexture->BindTexture(0);
		mat.specMap->BindTexture(1);
		shaderProgram->SetUniform3FV("material.diffColour", 1, &mat.diffColour.r);
		shaderProgram->SetUniform3FV("material.specColour", 1, &mat.specColour.r);

		mesh.Draw();
	}*/
}

OpenGL::Buffer& Renderer::CreateLivingBuffer() {
	return m_livingBuffers.emplace_back();

}
