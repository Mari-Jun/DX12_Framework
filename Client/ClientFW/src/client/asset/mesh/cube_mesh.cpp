#include "stdafx.h"
#include "client/asset/mesh/cube_mesh.h"

namespace client_fw
{
	CubeMesh::CubeMesh()
	{
		m_primitive_topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		m_vertex_info = CreateSPtr<VertexInfo>();
		m_index_info = CreateSPtr<IndexInfo>();
	}

	CubeMesh::~CubeMesh()
	{
	}

	void CubeMesh::Shutdown()
	{
	}

	bool SkyCubeMesh::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		std::array<Vertex, 8> vertices;
		std::array<UINT, 36> indices;

		float extent = 50.0f;

		vertices[0].SetPosition(Vec3(-extent, extent, -extent));
		vertices[1].SetPosition(Vec3(extent, extent, -extent));
		vertices[2].SetPosition(Vec3(extent, extent, extent));
		vertices[3].SetPosition(Vec3(-extent, extent, extent));
		vertices[4].SetPosition(Vec3(-extent, -extent, -extent));
		vertices[5].SetPosition(Vec3(extent, -extent, -extent));
		vertices[6].SetPosition(Vec3(extent, -extent, extent));
		vertices[7].SetPosition(Vec3(-extent, -extent, extent));

		m_vertex_info->CreateVertexBlob<Vertex>(vertices.size());
		m_vertex_info->CopyData(vertices.data(), vertices.size());

		indices[0] = 3; indices[1] = 1; indices[2] = 0;
		indices[3] = 2; indices[4] = 1; indices[5] = 3;
		indices[6] = 0; indices[7] = 5; indices[8] = 4;
		indices[9] = 1; indices[10] = 5; indices[11] = 0;
		indices[12] = 3; indices[13] = 4; indices[14] = 7;
		indices[15] = 0; indices[16] = 4; indices[17] = 3;
		indices[18] = 1; indices[19] = 6; indices[20] = 5;
		indices[21] = 2; indices[22] = 6; indices[23] = 1;
		indices[24] = 2; indices[25] = 7; indices[26] = 6;
		indices[27] = 3; indices[28] = 7; indices[29] = 2;
		indices[30] = 6; indices[31] = 4; indices[32] = 5;
		indices[33] = 7; indices[34] = 4; indices[35] = 6;

		m_index_info->CreateIndexBlob(indices.size());
		m_index_info->CopyData(indices.data(), indices.size());

		if (m_vertex_info->Initialize<Vertex>(device, command_list) == false)
		{
			LOG_ERROR("Could not create sky cube vertex buffer");
			return false;
		}

		if (m_index_info->Initialize(device, command_list) == false)
		{
			LOG_ERROR("Could not create sky cube index buffer");
			return false;
		}

		return true;
	}

	void SkyCubeMesh::Draw(ID3D12GraphicsCommandList* command_list, UINT num_of_draw_data, UINT lod) const
	{
		command_list->IASetPrimitiveTopology(m_primitive_topology);

		m_vertex_info->Draw(command_list);
		m_index_info->Draw(command_list);

		command_list->DrawIndexedInstanced(36, num_of_draw_data, 0, 0, 0);
	}
}
