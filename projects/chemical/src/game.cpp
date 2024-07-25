#include <pch.h>

#include "game.h"
#include "rendering/model_importer.h"

Game::Game(Chemical::ChemicalEngine& engine) {

	Chemical::ModelImporter importer;

	auto m = importer.ImportModel("resources/models/stanford-dragon.fbx");
	scene.AddMesh(m.value());

	auto m2 = importer.ImportModel("resources/models/stanford-bunny.fbx");
	scene.AddMesh(m2.value());

	m.value()->transform.position.x += 15;
}

void Game::Update(Chemical::ChemicalEngine& engine) {
	engine.renderer->RenderScene(scene);
}