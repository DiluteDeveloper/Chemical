#pragma once

#include <vector>


namespace Chemical {

	class Scene;
	class Object {
	public:

		virtual void OnSceneLoad() {}
		virtual void OnSceneUpdate() {}
		virtual void OnSceneUnload() {}

		bool enabled = true;

		// copies child
		void AddChild(const Object& child) {
			m_children.emplace_back(child);
		}
		std::vector<Object>& GetChildren() {
			return m_children;
		}
		Scene& GetScene() {
			if(scene != nullptr)
				return *scene;
			else
				throw std::exception();
		}

	private:
		friend class Scene;

		Scene* scene = nullptr;
			
		std::vector<Object> m_children;
	};

}
