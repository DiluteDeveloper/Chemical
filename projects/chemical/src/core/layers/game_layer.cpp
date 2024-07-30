#include <pch.h>

#include "game_layer.h"
#include "scene_layer.h"
#include "rendering/model_importer.h"

using namespace Chemical;

void GameLayer::InitializeLayer() {

	Chemical::ModelImporter importer;
	auto m = importer.ImportModel("resources/models/stanford-dragon.fbx");
	scene.AddMesh(m.value());

	auto m2 = importer.ImportModel("resources/models/stanford-bunny.fbx");
	scene.AddMesh(m2.value());

	m.value()->transform.position.x += 15;

	Core::SceneLayer* scene_layer = m_app_data.GetLayer<Core::SceneLayer>("SceneLayer");
	if(scene_layer != nullptr)
		scene_layer->SetScene(&scene);
}