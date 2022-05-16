#include "stdafx.h"
#include "client/asset/primitive/primitive.h"

namespace client_fw
{
	bool IndexInfo::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		auto indices = reinterpret_cast<UINT*>(m_index_buffer_blob->GetBufferPointer());
		auto indices_size = static_cast<UINT>(m_index_buffer_blob->GetBufferSize());

		m_index_buffer = D3DUtil::CreateDefaultBuffer(device, command_list, indices, indices_size,
			D3D12_RESOURCE_STATE_INDEX_BUFFER, m_index_upload_buffer);

		m_index_buffer_view.BufferLocation = m_index_buffer->GetGPUVirtualAddress();
		m_index_buffer_view.Format = DXGI_FORMAT_R32_UINT;
		m_index_buffer_view.SizeInBytes = indices_size;

		return (m_index_buffer != nullptr);
	}

	void IndexInfo::Draw(ID3D12GraphicsCommandList* command_list) const
	{
		command_list->IASetIndexBuffer(&m_index_buffer_view);
	}

	bool IndexInfo::CreateIndexBlob(SIZE_T count)
	{
		return SUCCEEDED(D3DCreateBlob(sizeof(UINT) * count, m_index_buffer_blob.GetAddressOf()));
	}

	void IndexInfo::CopyData(UINT* indices, size_t count)
	{
		CopyMemory(m_index_buffer_blob->GetBufferPointer(), indices, sizeof(UINT) * count);
	}
}
