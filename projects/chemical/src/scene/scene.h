#pragma once

#include <memory>

#include "core/application.h"
#include "camera_3d.h"
#include "mesh_3d.h"

namespace Chemical {
	namespace Core {
		class SceneLayer;
	}
	class Scene {
	public:

		Core::GLFWWrapper& GetGLFWWrapper() const {
			return m_glfw_wrapper;
		}
		Core::EventDispatcher& GetEventDispatcher() const {
			return m_dispatcher;
		}

		void AddMesh3D(Mesh3D&& mesh) {
			m_meshes3D.push_back(std::move(mesh));
			Mesh3D& m = m_meshes3D[m_meshes3D.size() - 1];
			m.scene = this;
			m.OnSceneLoad();
		}
		std::vector<Mesh3D>& GetMeshes3D() {
			return m_meshes3D;
		}
		Camera3D& GetCamera3D() {
			return m_camera3D;
		}
		Scene(Core::GLFWWrapper& glfw_wrapper, Core::EventDispatcher& dispatcher) :
			m_glfw_wrapper(glfw_wrapper), m_dispatcher(dispatcher) {
			m_camera3D.scene = this;
			m_camera3D.OnSceneLoad();
		}

		~Scene() {
			for (Mesh3D& m : m_meshes3D) {
				m.OnSceneUnload();
			}
			m_camera3D.OnSceneUnload();
		}

	private:
		friend class Chemical::Core::SceneLayer;
		Core::GLFWWrapper& m_glfw_wrapper;
		Core::EventDispatcher& m_dispatcher;

		void Update() {
			for (Mesh3D& m : m_meshes3D) {
				m.OnSceneUpdate();
			}
			m_camera3D.OnSceneUpdate();

		}

		std::vector<Mesh3D> m_meshes3D;
		Camera3D m_camera3D;
	};
}
