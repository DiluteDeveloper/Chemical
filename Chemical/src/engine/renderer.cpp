#include "renderer.h"
#include "mesh.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vendor/IMGUI/imgui.h>

Renderer::Renderer(Scene& in_scene) :
	litShader("res/shaders/litShader.v", "res/shaders/litShader.f"),
	unlitShader("res/shaders/unlitShader.v", "res/shaders/unlitShader.f")
{
	scene = &in_scene;
}

Renderer::Renderer() :
	litShader("res/shaders/litShader.v", "res/shaders/litShader.f"),
	unlitShader("res/shaders/unlitShader.v", "res/shaders/unlitShader.f")
{ }

void Renderer::bindScene(Scene& in_scene) {
	scene = &in_scene;

	litShader.bind();
	litShader.setMatrix4fv("v_proj", scene->camera.proj);
	unlitShader.bind();
	unlitShader.setMatrix4fv("v_proj", scene->camera.proj);
}

void Renderer::render() const {

	if (scene == nullptr) {
		std::cout << "Renderer::render() failed. scene is nullptr." << std::endl;
		return;
	}

	litShader.bind();

	glm::mat4 inv = glm::inverse(scene->camera.transform.toMat4());
	litShader.setMatrix4fv("v_view", inv);
	litShader.setVector3("viewPos", scene->camera.transform.position);

	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{

		std::string index = std::to_string(i);
		std::string obj = "pointLights[";

		litShader.setVector3(obj + index + "].position", scene->getPointLight(i)->transform.position);
		litShader.setVector3(obj + index + "].diffuse", scene->getPointLight(i)->diffuse);
		litShader.setVector3(obj + index + "].ambient", scene->getPointLight(i)->ambient);
		litShader.setVector3(obj + index + "].specular", scene->getPointLight(i)->specular);
	}

	for (size_t i = 0; i < scene->getRenderablesSize(); i++)
	{
		litShader.setVector3("material.ambient", scene->getRenderable(i)->material.ambient);
		litShader.setVector3("material.diffuse", scene->getRenderable(i)->material.diffuse);
		litShader.setVector3("material.specular", scene->getRenderable(i)->material.specular);
		litShader.setFloat("material.shininess", scene->getRenderable(i)->material.shininess);

		litShader.setMatrix4fv("v_model", scene->getRenderable(i)->transform.toMat4());

		scene->getRenderable(i)->mesh.draw();
	}

	unlitShader.bind();

	unlitShader.setMatrix4fv("v_view", inv);

	for (size_t i = 0; i < scene->getPointLightsSize(); i++)
	{
		unlitShader.setMatrix4fv("v_model", scene->getPointLight(i)->transform.toMat4());
		unlitShader.setVector3("colour", scene->getPointLight(i)->diffuse);

		scene->getDefaultMesh().draw();
	}
}

std::string invUniqueGUID(void* address) {
	std::ostringstream oss;
	oss << address;
	return ("##" + oss.str());
}

std::string uniqueGUID(void* address) {
	std::ostringstream oss;
	oss << address;
	return oss.str();
}
using namespace ImGui;
bool renderRenderableGUI(Renderable& r) {

	bool result = false;

	PushItemWidth(250.0f);
	if (BeginCombo(invUniqueGUID(&r).c_str(), ("Renderable " + uniqueGUID(&r)).c_str())) {
		PushItemWidth(100.0f);
		if (BeginCombo(invUniqueGUID(&r.transform).c_str(), "Transform")) {
			DragFloat3(("Position" + invUniqueGUID(&r.transform.position)).c_str(), &r.transform.position[0], 0.02f, -2000000.0f, -2000000.0f, "%.2f");
			DragFloat3(("Rotation" + invUniqueGUID(&r.transform.rotation)).c_str(), &r.transform.rotation[0], 0.2f, -2000000.0f, -2000000.0f, "%.2f");
			DragFloat3(("Scale" + invUniqueGUID(&r.transform.scale)).c_str(), &r.transform.scale[0], 0.02f, -2000000.0f, -2000000.0f, "%.2f");

			EndCombo();
		}

		if (BeginCombo(invUniqueGUID(&r.material).c_str(), "Material")) {
			DragFloat3(("Ambient" + invUniqueGUID(&r.material.ambient)).c_str(), &r.material.ambient[0], 0.02f, 0.0f, 1.0f, "%.2f");
			DragFloat3(("Diffuse" + invUniqueGUID(&r.material.diffuse)).c_str(), &r.material.diffuse[0], 0.02f, 0.0f, 1.0f, "%.2f");
			DragFloat3(("Specular" + invUniqueGUID(&r.material.specular)).c_str(), &r.material.specular[0], 0.02f, 0.0f, 1.0f, "%.2f");
			DragFloat(("Shininess" + invUniqueGUID(&r.material.shininess)).c_str(), &r.material.shininess, 0.2f, 0.0f, 256.0f, "%.1f");

			EndCombo();
		}
		if (ImGui::Button(("Delete " + invUniqueGUID(&r)).c_str()))
			result = true;
		EndCombo();
	}
	PopItemWidth();
	return result;
}

bool renderPointLightGUI(PointLight& pl) {
	bool result = false;
	PushItemWidth(250.0f);
	if (BeginCombo(invUniqueGUID(&pl).c_str(), ("Point Light " + uniqueGUID(&pl)).c_str())) {
		PushItemWidth(100.0f);
		if (BeginCombo(invUniqueGUID(&pl.transform).c_str(), "Transform")) {
			DragFloat3(("Position" + invUniqueGUID(&pl.transform.position)).c_str(), &pl.transform.position[0], 0.02f, -2000000.0f, -2000000.0f, "%.2f");
			DragFloat3(("Rotation" + invUniqueGUID(&pl.transform.rotation)).c_str(), &pl.transform.rotation[0], 0.2f, -2000000.0f, -2000000.0f, "%.2f");
			DragFloat3(("Scale" + invUniqueGUID(&pl.transform.scale)).c_str(), &pl.transform.scale[0], 0.02f, -2000000.0f, -2000000.0f, "%.2f");

			EndCombo();
		}
		if (BeginCombo(invUniqueGUID(&pl).c_str(), "Colour")) {
			DragFloat3(("Ambient" + invUniqueGUID(&pl.ambient)).c_str(), &pl.ambient[0], 0.02f, 0.0f, 1.0f, "%.2f");
			DragFloat3(("Diffuse" + invUniqueGUID(&pl.diffuse)).c_str(), &pl.diffuse[0], 0.02f, 0.0f, 1.0f, "%.2f");
			DragFloat3(("Specular" + invUniqueGUID(&pl.specular)).c_str(), &pl.specular[0], 0.02f, 0.0f, 1.0f, "%.2f");

			EndCombo();
		}
		if (ImGui::Button(("Delete " + invUniqueGUID(&pl)).c_str()))
			result = true;

		EndCombo();
	}
	PopItemWidth();
	return result;
}

using namespace ImGui;

void Renderer::renderSceneGUI() const {
	Begin("Scene", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
	PushItemWidth(125.0f);

	if (BeginCombo("##renderables", "Renderables")) {
		for (int i = 0; i < scene->getRenderablesSize(); i++)
		{
			if (renderRenderableGUI(*scene->getRenderable(i)))
				scene->removeRenderableIndex(i);

		}
		if (ImGui::Button("Create Renderable"))
			scene->createRenderable();
		EndCombo();
	}

	PushItemWidth(125.0f);
	if (BeginCombo("##point_lights", "Point Lights")) {
		for (int i = 0; i < scene->getPointLightsSize(); i++) {
			if (renderPointLightGUI(*scene->getPointLight(i)))
				scene->removePointLightIndex(i);
		}
		if (ImGui::Button("Create Point Light"))
			scene->createPointLight();

		EndCombo();
	}

	PopItemWidth();
	End();
}