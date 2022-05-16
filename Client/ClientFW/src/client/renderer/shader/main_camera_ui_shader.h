#pragma once
#include "client/renderer/shader/core/graphics_shader.h"

namespace client_fw
{
	template<class T> class UploadBuffer;

	class MainCameraUIShader : public GraphicsShader
	{
	public:
		MainCameraUIShader(const std::string& name);
		virtual ~MainCameraUIShader() = default;

		virtual void Initialize(ID3D12Device* device) override;
		virtual void Shutdown() override;

		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const override;

		virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;

		D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState(eRenderLevelType level_type, int pso_index) const override;

		virtual bool CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level) override;
	};

}


