#include "stdafx.h"
#include "client/renderer/rootsignature/graphics_super_root_signature.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/util/d3d_util.h"

namespace client_fw
{
	GraphicsSuperRootSignature::GraphicsSuperRootSignature()
	{
	}

	GraphicsSuperRootSignature::~GraphicsSuperRootSignature()
	{
		
	}

	void GraphicsSuperRootSignature::Shutdown()
	{
	}

	void GraphicsSuperRootSignature::Draw(ID3D12GraphicsCommandList* command_list) const
	{
		command_list->SetGraphicsRootSignature(m_root_signature.Get());
	}

	bool GraphicsSuperRootSignature::CreateRootSignature(ID3D12Device* device)
	{
		std::array<CD3DX12_DESCRIPTOR_RANGE, 3> descriptor_range;
		descriptor_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, MAX_2D_TEXTURE_RESOURCE_SIZE, 0, 1);
		descriptor_range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, MAX_CUBE_TEXTURE_RESOURCE_SIZE, 0, 2);
		descriptor_range[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, MAX_ARRAY_TEXTURE_RESOURCE_SIZE, 0, 3);

		std::array<CD3DX12_ROOT_PARAMETER, 10> root_parameters;
		root_parameters[0].InitAsConstantBufferView(0, 0);
		root_parameters[1].InitAsShaderResourceView(0, 0);
		root_parameters[2].InitAsConstantBufferView(1, 0);
		root_parameters[3].InitAsShaderResourceView(1, 0);
		root_parameters[4].InitAsDescriptorTable(3, &descriptor_range[0]);
		root_parameters[5].InitAsShaderResourceView(2, 0);
		root_parameters[6].InitAsShaderResourceView(4, 0);
		root_parameters[7].InitAsShaderResourceView(6, 0);
		root_parameters[8].InitAsShaderResourceView(3, 0);
		root_parameters[9].InitAsShaderResourceView(5, 0);

		std::array<CD3DX12_STATIC_SAMPLER_DESC, 2> static_samplers;
		static_samplers[0].Init(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP);
		static_samplers[1].Init(6, D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT, D3D12_TEXTURE_ADDRESS_MODE_BORDER,
			D3D12_TEXTURE_ADDRESS_MODE_BORDER, D3D12_TEXTURE_ADDRESS_MODE_BORDER, 0.0f, 1, D3D12_COMPARISON_FUNC_LESS_EQUAL,
			D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE);

		CD3DX12_ROOT_SIGNATURE_DESC root_signature_desc(static_cast<UINT>(root_parameters.size()), root_parameters.data(),
			static_cast<UINT>(static_samplers.size()), static_samplers.data(),
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		ComPtr<ID3DBlob> signature_blob = nullptr;
		ComPtr<ID3DBlob> error_blob = nullptr;

		if (FAILED(D3D12SerializeRootSignature(&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1,
			signature_blob.GetAddressOf(), error_blob.GetAddressOf())))
		{
			return false;
		}

		if (error_blob != nullptr)
		{
			OutputDebugStringA(static_cast<char*>(error_blob->GetBufferPointer()));
		}

		return SUCCEEDED(device->CreateRootSignature(0, signature_blob->GetBufferPointer(),
			signature_blob->GetBufferSize(), IID_PPV_ARGS(&m_root_signature)));
	}
}
