#pragma once
#include "client/util/d3d_util.h"
#include "client/util/upload_buffer.h"
#include "client/asset/primitive/vertex.h"

namespace client_fw
{
	class VertexInfo
	{
	public:
		VertexInfo() = default;

		template <class VertexType>
		bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
		{
			auto vertices = static_cast<VertexType*>(m_vertex_buffer_blob->GetBufferPointer());
			auto vertices_size = static_cast<UINT>(m_vertex_buffer_blob->GetBufferSize());

			m_vertex_buffer = D3DUtil::CreateDefaultBuffer(device, command_list, vertices, vertices_size,
				D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, m_vertex_upload_buffer);
			D3DUtil::SetObjectName(m_vertex_buffer.Get(), "vertex buffer");

			m_vertex_buffer_view.BufferLocation = m_vertex_buffer->GetGPUVirtualAddress();
			m_vertex_buffer_view.SizeInBytes = vertices_size;
			m_vertex_buffer_view.StrideInBytes = sizeof(VertexType);

			return (m_vertex_buffer != nullptr);
		}

		void Draw(ID3D12GraphicsCommandList* command_list) const
		{
			command_list->IASetVertexBuffers(m_slot, 1, &m_vertex_buffer_view);
		}

		template <class VertexType>
		bool CreateVertexBlob(SIZE_T count)
		{
			return SUCCEEDED(D3DCreateBlob(sizeof(VertexType) * count, m_vertex_buffer_blob.GetAddressOf()));
		}

		template <class VertexType>
		void CopyData(VertexType* vertices, size_t count)
		{
			CopyMemory(m_vertex_buffer_blob->GetBufferPointer(), vertices, sizeof(VertexType) * count);
		}

	private:
		ComPtr<ID3DBlob> m_vertex_buffer_blob;
		ComPtr<ID3D12Resource> m_vertex_buffer;
		ComPtr<ID3D12Resource> m_vertex_upload_buffer;

		UINT m_slot = 0;
		D3D12_VERTEX_BUFFER_VIEW m_vertex_buffer_view;

	public:
		template <class VertexType>
		VertexType* GetVertices() const { return static_cast<VertexType*>(m_vertex_buffer_blob->GetBufferPointer()); }

		template <class VertexType>
		UINT GetVertexCount() const { return static_cast<UINT>(m_vertex_buffer_blob->GetBufferSize() / sizeof(VertexType)); }
	};

	class IndexInfo
	{
	public:
		IndexInfo() = default;

		bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Draw(ID3D12GraphicsCommandList* command_list) const;
		bool CreateIndexBlob(SIZE_T count);
		void CopyData(UINT* indices, size_t count);

	private:
		ComPtr<ID3DBlob> m_index_buffer_blob;
		ComPtr<ID3D12Resource> m_index_buffer;
		ComPtr<ID3D12Resource> m_index_upload_buffer;

		D3D12_INDEX_BUFFER_VIEW m_index_buffer_view;

	public:
		UINT* GetIndices() const { return static_cast<UINT*>(m_index_buffer_blob->GetBufferPointer()); }
		UINT GetVertexCount() const { return static_cast<UINT>(m_index_buffer_blob->GetBufferSize() / sizeof(UINT)); }
	};

	class UploadVertexInfo
	{
	public:
		UploadVertexInfo() = default;

		template <class VertexType>
		bool CreateResource(ID3D12Device* device, UINT num_of_data)
		{
			Shutdown();

			m_vertex_buffer = D3DUtil::CreateUploadBuffer(device, sizeof(VertexType) * num_of_data,
				D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_vertex_mapped_data);
			D3DUtil::SetObjectName(m_vertex_buffer.Get(), "upload vertex buffer");

			m_vertex_buffer_view.BufferLocation = m_vertex_buffer->GetGPUVirtualAddress();
			m_vertex_buffer_view.SizeInBytes = sizeof(VertexType) * num_of_data;
			m_vertex_buffer_view.StrideInBytes = sizeof(VertexType);

			return (m_vertex_buffer != nullptr);
		}
		
		void Shutdown()
		{
			if (m_vertex_buffer != nullptr)
			{
				m_vertex_buffer->Unmap(0, nullptr);
			}
			m_vertex_buffer = nullptr;
			m_vertex_mapped_data = nullptr;
		}

		void Draw(ID3D12GraphicsCommandList* command_list) const
		{
			command_list->IASetVertexBuffers(m_slot, 1, &m_vertex_buffer_view);
		}

		template <class VertexType>
		void CopyData(VertexType* vertices, size_t count)
		{
			memcpy(m_vertex_mapped_data, vertices, sizeof(VertexType) * count);
		}

		template <class VertexType>
		void CopyData(const VertexType& data, UINT index)
		{
			memcpy(&m_vertex_mapped_data[index * sizeof(VertexType)], &data, sizeof(VertexType));
		}

	private:
		ComPtr<ID3D12Resource> m_vertex_buffer;
		BYTE* m_vertex_mapped_data;
		UINT m_slot = 0;
		
		D3D12_VERTEX_BUFFER_VIEW m_vertex_buffer_view;
	};

	class Primitive
	{
	public:
		Primitive() = default;
		virtual ~Primitive() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;
		virtual void Shutdown() = 0;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT num_of_draw_data, UINT lod = 0) const = 0;
		virtual void DrawForShadow(ID3D12GraphicsCommandList* command_list, UINT num_of_draw_data, UINT lod = 0) const {}

	protected:
		D3D12_PRIMITIVE_TOPOLOGY m_primitive_topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	public:
		void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology) { m_primitive_topology = topology; }
	};

	template<class VertexType>
	class UploadPrimitive
	{
	public:
		UploadPrimitive()
		{
			m_vertex_info = CreateSPtr<UploadVertexInfo>();
		}
		virtual ~UploadPrimitive() = default;

		virtual bool Initialize(ID3D12Device* device) { return true; }

		virtual void Shutdown()
		{ 
			m_vertex_info->Shutdown();
		}

		virtual void Update(ID3D12Device* device, UINT num_of_data)
		{
			m_vertex_info->CreateResource<VertexType>(device, num_of_data);
		}

		virtual void UpdateVertices(const std::vector<VertexType>& vertices)
		{
			m_vertex_info->CopyData(vertices.data(), vertices.size());
		}

		virtual void PreDraw(ID3D12GraphicsCommandList* command_list) const
		{
			command_list->IASetPrimitiveTopology(m_primitive_topology);
			m_vertex_info->Draw(command_list);
		}

		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT num_of_vertex) const
		{
			command_list->DrawInstanced(num_of_vertex, 1, 0, 0);
		}

		virtual void Draw(ID3D12GraphicsCommandList* command_list, UINT num_of_vertex, UINT start_vertex_location) const
		{
			command_list->DrawInstanced(num_of_vertex, 1, start_vertex_location, 0);
		}

	protected:
		SPtr<UploadVertexInfo> m_vertex_info;
		D3D12_PRIMITIVE_TOPOLOGY m_primitive_topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	public:
		void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology) { m_primitive_topology = topology; }
	};
}

