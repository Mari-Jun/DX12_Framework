#pragma once

namespace client_fw
{
	class D3DUtil final
	{
	public:
		static UINT s_cbvsrvuav_descirptor_increment_size;
		static UINT s_rtv_descirptor_increment_size;
		static UINT s_dsv_descirptor_increment_size;

		static bool s_is_use_4x_mass;
		static UINT s_4x_msaa_quality;


	public:
		static inline void SetObjectName(ID3D12Object* object, std::string name)
		{
#ifdef _DEBUG
			object->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(name.length()), name.data());
#endif
		}

		static inline void SetObjectName(IDXGIObject* object, std::string name)
		{
#ifdef _DEBUG
			object->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(name.length()), name.data());
#endif
		}

		static ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* command_list,
			const void* init_data, UINT byte_size, D3D12_RESOURCE_STATES resource_state, ComPtr<ID3D12Resource>& upload_buffer);

		static ComPtr<ID3D12Resource> CreateUploadBuffer(ID3D12Device* device, UINT byte_size, BYTE** mapped_data);
		static ComPtr<ID3D12Resource> CreateUploadBuffer(ID3D12Device* device, UINT byte_size, 
			D3D12_RESOURCE_STATES resource_state, BYTE** mapped_data);
	};
}

