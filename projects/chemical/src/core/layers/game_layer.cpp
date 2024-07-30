#include <pch.h>

#include "game_layer.h"
#include "scene_layer.h"
#include "rendering/model_importer.h"

void GameLayer::InitializeLayer() {

	Chemical::ModelImporter importer;

	auto m = importer.ImportModel("resources/models/stanford-dragon.fbx");
	scene.AddMesh(m.value());

	auto m2 = importer.ImportModel("resources/models/stanford-bunny.fbx");
	scene.AddMesh(m2.value());

	m.value()->transform.position.x += 15;

	m_app_data.GetSceneLayer().SetScene(&scene);
}