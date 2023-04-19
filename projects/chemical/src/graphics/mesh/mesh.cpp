#include "pch.h"
#include "mesh.h"

StaticMeshData::StaticMeshData(const Util::TypelessArray& vertexData, const Util::TypelessArray& elementData)
	: m_vertexData(vertexData), m_elementData(elementData) {
	m_vertexInfo.bindingIndex = 0;
	m_vertexInfo.offset = 0;
	m_vertexInfo.stride = (uint32_t)m_vertexData.type_size();

	m_drawInfo.count = (int32_t)m_elementData.len();
	m_drawInfo.offset = (int64_t)m_vertexData.arr_data_size();

	switch (m_elementData.type_size()) {
	case 1:
		m_drawInfo.dataType = OpenGL::DataType::UNSIGNED_BYTE;
		break;
	case 2:
		m_drawInfo.dataType = OpenGL::DataType::UNSIGNED_SHORT;
		break;
	case 4:
		m_drawInfo.dataType = OpenGL::DataType::UNSIGNED_INT;
		break;
	default:
		LOGGER_CONSOLE_ERROR("Invalid data type supplied to StaticMeshData.");
	}
}

void StaticMeshData::SetVertexData(const Util::TypelessArray& data) {
	m_vertexData = data;
	m_vertexInfo.stride = (int32_t)data.type_size();
	m_drawInfo.offset = data.arr_data_size();
}
void StaticMeshData::SetElementData(const Util::TypelessArray& data) {
	m_elementData = data;
	m_drawInfo.count = (int32_t)m_elementData.len();

	switch (m_elementData.type_size()) {
	case 1:
		m_drawInfo.dataType = OpenGL::DataType::UNSIGNED_BYTE;
		break;
	case 2:
		m_drawInfo.dataType = OpenGL::DataType::UNSIGNED_SHORT;
		break;
	case 4:
		m_drawInfo.dataType = OpenGL::DataType::UNSIGNED_SHORT;
		break;
	default:
		LOGGER_CONSOLE_ERROR("Invalid data type supplied to StaticMeshData.");
	}

}

StaticMeshData::StaticMeshData() { m_vertexInfo.bindingIndex = 0; m_drawInfo.offset = 0; }

void StaticMeshData::AddVertexAttribute(const OpenGL::VertexAttribute attribute) {
	m_vertexInfo.AddAttribute(attribute);
}

void StaticMeshData::SetDrawMode(OpenGL::DrawMode mode) {
	m_drawInfo.mode = mode;
}

StaticMesh::StaticMesh(const StaticMeshData& data) : m_info(data.m_drawInfo) {
	OpenGL::Buffer buffer;

	buffer.CreateImmutableBuffer(data.m_vertexData.arr_data_size() + data.m_elementData.arr_data_size(), nullptr, OpenGL::BufferStorageFlags::DYNAMIC_STORAGE_BIT);

	buffer.SetBufferData(data.m_vertexData.arr_data_size(), data.m_vertexData.data());
	buffer.SetBufferData(data.m_elementData.arr_data_size(), data.m_elementData.data(), (uint32_t)data.m_vertexData.arr_data_size());

	m_vArray.SetVertexBuffer(buffer, data.m_vertexInfo);
	m_vArray.SetElementBuffer(buffer);
}

void StaticMesh::Draw() {
	m_vArray.Bind();
	m_vArray.DrawElements(m_info);
}

void StaticMesh::SetDrawMode(OpenGL::DrawMode mode) {
	m_info.mode = mode;
}