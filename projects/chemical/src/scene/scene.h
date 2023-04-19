#pragma once

#include <entt/entt.h>


class Entity {
	entt::registry* m_registry;
	entt::entity m_entity;
public:

	Entity(entt::entity entity, entt::registry* registry) :
		m_entity(entity), m_registry(registry) {}

	// Can return nullptr if component already exists in entity
	template<typename T, typename... ARGS>
	T* AddComponent(ARGS&&... args) {
		if (!m_registry->any_of<T>(m_entity)) {
			return &m_registry->emplace<T>(m_entity, std::forward<ARGS>(args)...);
		}
		else {
			LOGGER_CONSOLE_CUSTOM_WARNING("Component already exists on entity {}.", (uint16_t)m_entity);
			return nullptr;
		}
	}

	// Can return nullptr if entity does not contain component
	template<typename T>
	T* GetComponent() {
		return m_registry->try_get<T>(m_entity);
	}

	// Will safely return if entity does not have component
	template<typename T>
	void RemoveComponent() {
		m_registry->remove<T>(m_entity);
	}
};

class Scene {
	friend class Renderer;

	entt::registry m_registry;
public:

	Scene() = default;

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	Entity CreateEntity();
};
