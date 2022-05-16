#pragma once
#include "client/asset/primitive/primitive.h"

namespace client_fw
{
	class CubeMesh : public Primitive
	{
	protected:
		CubeMesh();
		virtual ~CubeMesh();

	public:
		virtual void Shutdown() override;
	
	protected:
		SPtr<VertexInfo> m_vertex_info;
		SPtr<IndexInfo> m_index_info;
	};

	class SkyCubeMesh : public CubeMesh
	{
	public:
		SkyCubeMesh() = default;
		virtual ~SkyCubeMesh() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT num_of_draw_data = 1, UINT lod = 0) const override;
	};
}


