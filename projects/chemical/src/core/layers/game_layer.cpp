#include <pch.h>

#include "game_layer.h"
#include "scene_layer.h"
#include "rendering/model_importer.h"

using namespace Chemical;

void GameLayer::InitializeLayer() {

	Chemical::ModelImporter importer;
	root = importer.ImportModel("resources/models/stanford-dragon.fbx");
	if (root != nullptr)
		m_app_data.GetLayer<Core::SceneLayer>("SceneLayer")->SetRoot(&root->object);

	camera_3d = std::make_unique<Scene::Camera3D>(m_app_data.GetGLFWWrapper(), m_app_data.GetDispatcher());
	m_app_data.GetLayer<Core::SceneLayer>("SceneLayer")->SetCamera3D(camera_3d.get());

	//Core::SceneLayer* scene_layer = m_app_data.GetLayer<Core::SceneLayer>("SceneLayer");
	//if(scene_layer != nullptr)
	//	scene_layer->SetScene(&scene);
};