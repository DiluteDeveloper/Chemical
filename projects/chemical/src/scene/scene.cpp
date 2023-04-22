#include "pch.h"
#include "scene.h"

Entity Scene::CreateEntity() {

	return Entity(m_registry.create(), &m_registry);

	/*using namespace entt::literals;
	LOGGER_CONSOLE_MESSAGE("BEFORE");
	auto&& other = m_registry.storage <uint32_t> ("other"_hs);
	LOGGER_CONSOLE_MESSAGE("AFTER");
	uint32_t& et = other.emplace(e, 1);
	LOGGER_CONSOLE_CUSTOM_MESSAGE("{}", et);
	auto&& other2 = m_registry.storage <uint32_t>("other"_hs);
	uint32_t& et2 = other2.emplace(e2);
	//et.CreateComponent
	// test of named types
	return Entity(m_registry.create(), &m_registry);*/
}

void Scene::OrphanEntity(Entity& entity) {
	m_registry.release(entity.m_entity);
}

void Scene::DeleteEntity(Entity& entity) {
	m_registry.destroy(entity.m_entity);
}

void Scene::Reserve(size_t numEntities) {
	m_registry.reserve(numEntities);
}