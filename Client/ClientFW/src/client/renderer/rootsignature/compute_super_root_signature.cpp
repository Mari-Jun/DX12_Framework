#include "stdafx.h"
#include "client/renderer/rootsignature/compute_super_root_signature.h"
#include "client/util/d3d_util.h"

namespace client_fw
{
	ComputeSuperRootSignature::ComputeSuperRootSignature()
	{

	}
	ComputeSuperRootSignature::~ComputeSuperRootSignature()
	{
	}

	void ComputeSuperRootSignature::Shutdown()
	{
	}

	void ComputeSuperRootSignature::Draw(ID3D12GraphicsCommandList* command_list) const
	{
		command_list->SetComputeRootSignature(m_root_signature.Get());
	}

	bool ComputeSuperRootSignature::CreateRootSignature(ID3D12Device* device)
	{
		std::array<CD3DX12_DESCRIPTOR_RANGE, 2> descriptor_range;
		descriptor_range[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		descriptor_range[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);

		std::array<CD3DX12_ROOT_PARAMETER, 2> root_parameters;
		root_parameters[0].InitAsDescriptorTable(1, &descriptor_range[0]);
		root_parameters[1].InitAsDescriptorTable(1, &descriptor_range[1]);

		std::array<CD3DX12_STATIC_SAMPLER_DESC, 0> static_samplers;

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
