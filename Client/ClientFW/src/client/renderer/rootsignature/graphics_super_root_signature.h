#pragma once
#include "client/renderer/rootsignature/root_signature.h"

namespace client_fw
{
	class CameraComponent;

	class GraphicsSuperRootSignature final : public RootSignature
	{
	public:
		GraphicsSuperRootSignature();
		virtual ~GraphicsSuperRootSignature();

		virtual void Shutdown() override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list) const override;

		virtual bool CreateRootSignature(ID3D12Device* device) override;
	};
}


