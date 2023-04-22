#pragma once

#include <entt/entt.h>

class HashedString {
	entt::hashed_string m_hash;
public:
	HashedString(std::string_view hash) :
	m_hash(hash.data()){ }

	std::string GetHashName() const { return m_hash.data(); }

	uint32_t GetHash() const { return m_hash; }

	operator uint32_t() const {
		return m_hash;
	}
};

template<> struct std::hash<HashedString>
{
	uint32_t operator()(HashedString const& h) const noexcept {
		return h;
	}
};





class Entity {
	friend class Scene;

	entt::registry* m_registry;
	entt::entity m_entity;
public:

	Entity(entt::entity entity, entt::registry* registry) :
		m_entity(entity), m_registry(registry) {}

	// Can return nullptr if component already exists in entity
	template<typename T, typename... ARGS>
	T* CreateComponent(ARGS&&... args) {
		if (!m_registry->valid(m_entity)) {
			LOGGER_CONSOLE_ERROR("entity is null.");
			return nullptr;
		}


		if (!m_registry->any_of<T>(m_entity)) {
			return &m_registry->emplace<T>(m_entity, std::forward<ARGS>(args)...);
		}
		else {
			LOGGER_CONSOLE_CUSTOM_WARNING("Component already exists on entity {}.", (uint16_t)m_entity);
			return nullptr;
		}
	}
	template<typename T, typename... ARGS>
	T* CreateNamedComponent(HashedString hash, ARGS&&... args) {
		if (!m_registry->valid(m_entity)) {
			LOGGER_CONSOLE_ERROR("entity is null.");
			return nullptr;
		}


		if (!m_registry->any_of<T>(m_entity)) {

			auto&& storage = m_registry->storage<T>(hash);
			return &storage.emplace(m_entity, std::forward<ARGS>(args)...);
		}
		else {
			LOGGER_CONSOLE_CUSTOM_WARNING("Component already exists on entity {}.", (uint16_t)m_entity);
			return nullptr;
		}
	}

	// Can return nullptr if entity does not contain component
	template<typename T>
	T* GetComponent() {
		if (!m_registry->valid(m_entity)) {
			LOGGER_CONSOLE_ERROR("entity is null.");
			return nullptr;
		}

		return m_registry->try_get<T>(m_entity);
	}

	// Will safely return if entity does not have component
	template<typename T>
	void RemoveComponent() {
		if (m_registry == nullptr) {
			LOGGER_CONSOLE_ERROR("registry is null.");
			return nullptr;
		}
		m_registry->remove<T>(m_entity);
	}
};

class Scene {
	friend class Renderer;

	entt::registry m_registry;
public:

	Scene() = default;

	Entity CreateEntity();

	void OrphanEntity(Entity& entity);

	void DeleteEntity(Entity& entity);

	// increases the memory capacity to existing amount of entities + numEntities
	void Reserve(size_t numEntities);

	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
};
