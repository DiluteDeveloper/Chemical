#include <pch.h>

#include "game_layer.h"
#include "scene_layer.h"
#include "rendering/model_importer.h"

using namespace Chemical;

void GameLayer::InitializeLayer(Chemical::Core::ApplicationData& appData) {

	Core::SceneLayer* sceneLayer = appData.GetLayer<Core::SceneLayer>("SceneLayer");
	if (sceneLayer != nullptr) {
		ModelImporter importer;

		std::unique_ptr<GeneratedMesh3D> mesh = importer.ImportModel("resources/models/stanford-dragon.fbx");

		if (mesh != nullptr) {
			sceneLayer->scene->AddMesh3D(std::move(Mesh3D(std::move(*mesh.get()))));
		}

	}



};