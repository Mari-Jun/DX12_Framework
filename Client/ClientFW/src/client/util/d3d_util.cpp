#include "stdafx.h"
#include "client/util/d3d_util.h"

namespace client_fw
{
	UINT D3DUtil::s_cbvsrvuav_descirptor_increment_size = 0;
	UINT D3DUtil::s_rtv_descirptor_increment_size = 0;
	UINT D3DUtil::s_dsv_descirptor_increment_size = 0;

	bool D3DUtil::s_is_use_4x_mass = false; 
	UINT D3DUtil::s_4x_msaa_quality = 0;

	ComPtr<ID3D12Resource> D3DUtil::CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* command_list,
		const void* init_data, UINT byte_size, D3D12_RESOURCE_STATES resource_state, ComPtr<ID3D12Resource>& upload_buffer)
	{
		ComPtr<ID3D12Resource> buffer;

		if (FAILED(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(byte_size),
			D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(buffer.GetAddressOf()))))
		{
			LOG_ERROR("Could not create committed resource");
			return nullptr;
		}

		if (FAILED(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(byte_size),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(upload_buffer.GetAddressOf()))))
		{
			LOG_ERROR("Could not create committed resource");
			return nullptr;
		}

		D3D12_SUBRESOURCE_DATA sub_resource_data = {};
		sub_resource_data.pData = init_data;
		sub_resource_data.SlicePitch = sub_resource_data.RowPitch = byte_size;
		UpdateSubresources<1>(command_list, buffer.Get(), upload_buffer.Get(), 0, 0, 1, &sub_resource_data);

		command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(buffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, resource_state));

		return buffer;
	}

	ComPtr<ID3D12Resource> D3DUtil::CreateUploadBuffer(ID3D12Device* device, UINT byte_size, BYTE** mapped_data)
	{
		ComPtr<ID3D12Resource> buffer;

		if (FAILED(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(byte_size),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(buffer.GetAddressOf()))))
		{
			LOG_ERROR("Could not create committed resource");
			return nullptr;
		}

		if (FAILED(buffer->Map(0, nullptr, reinterpret_cast<void**>(mapped_data))))
		{
			LOG_ERROR("Could not mapped data");
			return nullptr;
		}
		
		return buffer;
	}

	ComPtr<ID3D12Resource> D3DUtil::CreateUploadBuffer(ID3D12Device* device, UINT byte_size,
		D3D12_RESOURCE_STATES resource_state, BYTE** mapped_data)
	{
		ComPtr<ID3D12Resource> buffer;

		if (FAILED(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(byte_size),
			resource_state | D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(buffer.GetAddressOf()))))
		{
			LOG_ERROR("Could not create committed resource");
			return nullptr;
		}

		if (FAILED(buffer->Map(0, nullptr, reinterpret_cast<void**>(mapped_data))))
		{
			LOG_ERROR("Could not mapped data");
			return nullptr;
		}

		return buffer;
	}
}