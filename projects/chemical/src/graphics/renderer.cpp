#include "pch.h"
#include "renderer.h"

#include "mesh/mesh.h"

void Renderer::GenerateSceneRenderData(const Scene& scene) {

	m_sceneRenderData.clear();



	for (const auto& [key, value] : m_shaders) {
		entt::basic_view staticMeshes { scene.m_registry.storage<StaticMesh>(key) };

		OpenGL::Buffer vBuffer;
		OpenGL::Buffer eBuffer;

		m_sceneRenderData[key].staticVArray.SetVertexBuffer(vBuffer, value->GetLayout(), 0, 0);
		m_sceneRenderData[key].staticVArray.SetElementBuffer(eBuffer);

		int32_t vSize = 0;
		int32_t eSize = 0;
		uint32_t count = 0;

		for (const auto&[entity, mesh] : staticMeshes.each()) {
			vSize += (int32_t)mesh.vertexData.arr_data_size();
			eSize += (int32_t)mesh.elementData.arr_data_size();
			count += (uint32_t)mesh.elementData.len();
		}
		vBuffer.CreateImmutableBuffer(vSize, nullptr, OpenGL::BufferStorageFlags::DYNAMIC_STORAGE_BIT);
		eBuffer.CreateImmutableBuffer(eSize, nullptr, OpenGL::BufferStorageFlags::DYNAMIC_STORAGE_BIT);
		m_sceneRenderData[key].count = count;
	
		uint32_t curVOffset = 0;
		uint32_t curEOffset = 0;

		for (const auto& [entity, mesh] : staticMeshes.each()) {
			vBuffer.SetBufferData(mesh.vertexData.arr_data_size(), mesh.vertexData.data(), curVOffset);
			curVOffset += (uint32_t)mesh.vertexData.arr_data_size();
			eBuffer.SetBufferData(mesh.elementData.arr_data_size(), mesh.elementData.data(), curEOffset);
			curEOffset += (uint32_t)mesh.elementData.arr_data_size();
		}


	}

}

void Renderer::RenderSceneData() {
	for (const auto& [key, value] : m_sceneRenderData) {
		m_shaders[key]->BindProgram();

		value.staticVArray.Bind();

		OpenGL::ElementDrawInfo i;

		i.count = value.count;
		i.offset = 0;
		i.dataType = OpenGL::DataType::UNSIGNED_INT;
		i.mode = OpenGL::DrawMode::TRIANGLES;
		value.staticVArray.DrawElements(i);
	}
}
void Renderer::AddShader(HashedString h, const std::shared_ptr<OpenGL::ShaderProgram> program) {
	
	if (!m_shaders.contains(h)) {
		m_shaders[h] = program;
	}
	else {
		LOGGER_CONSOLE_CUSTOM_ERROR("RENDERER ALREADY CONTAINS SHADER {}", h.GetHashName());
	}

}
