#include "stdafx.h"
#include "client/asset/primitive/shape.h"
#include "client/asset/primitive/primitive.h"

namespace client_fw
{
	bool Shape::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		m_primitive_topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
		return true;
	}

	void Shape::Shutdown()
	{
		m_vertex_info->Shutdown();
	}

	void Shape::Udpate(ID3D12Device* device, UINT num_of_data)
	{
		m_vertex_info->CreateResource<ShapeVertex>(device, num_of_data);
	}

	void Shape::Draw(ID3D12GraphicsCommandList* command_list, UINT num_of_draw_data, UINT lod) const
	{
		m_vertex_info->Draw(command_list);
	}

	void Shape::AddShapeVertices(std::vector<ShapeVertex>&& vertices)
	{
		m_vertex_info->CopyData<ShapeVertex>(vertices.data(), vertices.size());
	}
}

