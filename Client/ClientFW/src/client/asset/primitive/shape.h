#pragma once
#include "client/asset/primitive/primitive.h"

namespace client_fw
{
	class ShapeVertex;

	class Shape : public Primitive
	{
	public:
		Shape() = default;
		virtual ~Shape() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;
		virtual void Shutdown() override;
		virtual void Udpate(ID3D12Device* device, UINT num_of_data);
		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT num_of_draw_data, UINT lod = 0) const override;

		virtual void AddShapeVertices(std::vector<ShapeVertex>&& vertices);

	protected:
		SPtr<UploadVertexInfo> m_vertex_info;
	};
}

