#pragma once
#include "client/renderer/rootsignature/root_signature.h"

namespace client_fw
{
	class ComputeSuperRootSignature : public RootSignature
	{
	public:
		ComputeSuperRootSignature();
		virtual ~ComputeSuperRootSignature();

		virtual void Shutdown() override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list) const override;

		virtual bool CreateRootSignature(ID3D12Device* device) override;
	};


}

