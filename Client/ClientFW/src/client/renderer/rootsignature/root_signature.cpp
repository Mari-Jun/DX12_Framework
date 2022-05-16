#include "stdafx.h"
#include "client/renderer/rootsignature/root_signature.h"

namespace client_fw
{
	bool RootSignature::Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		if (m_root_signature == nullptr)
		{
			if (CreateRootSignature(device) == false)
			{
				LOG_ERROR("Could not create root signature");
				return false;
			}
		}
		return true;
	}
}
