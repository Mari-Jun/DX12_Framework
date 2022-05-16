#pragma once
#include "client/renderer/shader/core/graphics_shader.h"

namespace client_fw
{
	class SkyRenderItem;

	class SkyShader : public GraphicsShader
	{
	public:
		SkyShader(const std::string& name);
		virtual ~SkyShader() = default;

		virtual void Initialize(ID3D12Device* device) override;
		virtual void Shutdown() override;

		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const override;

		virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_SHADER_BYTECODE CreateHullShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_SHADER_BYTECODE CreateDomainShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;

		virtual std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputLayout(eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_RASTERIZER_DESC CreateRasterizerState(eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState(eRenderLevelType level_type, int pso_index) const override;

		virtual D3D12_PRIMITIVE_TOPOLOGY_TYPE GetPrimitiveTopologyType(eRenderLevelType level_type, int pso_index) const;

		virtual bool CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level) override;

		virtual bool RegisterSkyComponent(ID3D12Device* device, const SPtr<SkyComponent>& sky_comp) override;
		virtual void UnregisterSkyComponent(const SPtr<SkyComponent>& sky_comp) override;

	protected:
		SPtr<SkyRenderItem> m_sky_render_item;
	};

}


