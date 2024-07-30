#pragma once

#include "core/application.h"

#include "scene_objects/mesh_3d.h"
#include "scene_objects/camera_3d.h"

class GameLayer : public Chemical::Core::ApplicationLayer {

private:
	using Chemical::Core::ApplicationLayer::ApplicationLayer;
	friend Chemical::Core::ApplicationData;

	void InitializeLayer() override;

	std::unique_ptr<Chemical::Scene::Mesh3D> root;
	std::unique_ptr<Chemical::Scene::Camera3D> camera_3d;
};
