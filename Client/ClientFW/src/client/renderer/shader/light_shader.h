#pragma once
#include "client/renderer/shader/core/graphics_shader.h"

namespace client_fw
{
	class LightRenderItem;

    class LightShader : public GraphicsShader
    {
    protected:
        LightShader(const std::string& name);
        virtual ~LightShader() = default;

		virtual void Initialize(ID3D12Device* device) override;
		virtual void Shutdown() override;

		virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;

		virtual std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputLayout(eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_RASTERIZER_DESC CreateRasterizerState(eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_BLEND_DESC CreateBlendState(eRenderLevelType level_type, int pso_index) const;
		virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState(eRenderLevelType level_type, int pso_index) const override;

		virtual D3D12_PRIMITIVE_TOPOLOGY_TYPE GetPrimitiveTopologyType(eRenderLevelType level_type, int pso_index) const;

		virtual bool CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level) override;

		virtual bool RegisterLocalLightComponent(ID3D12Device* device, const SPtr<LocalLightComponent> light_comp) override;
		virtual void UnregisterLocalLightComponent(const SPtr<LocalLightComponent>& light_comp) override;

	protected:
		SPtr<LightRenderItem> m_light_render_item;
    };

    class PointLightShader : public LightShader
    {
    public:
        PointLightShader(const std::string& name);
        virtual ~PointLightShader() = default;

		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const override;

		virtual D3D12_SHADER_BYTECODE CreateHullShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_SHADER_BYTECODE CreateDomainShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
    };

	class SpotLightShader : public LightShader
	{
	public:
		SpotLightShader(const std::string& name);
		virtual ~SpotLightShader() = default;

		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const override;

		virtual D3D12_SHADER_BYTECODE CreateHullShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_SHADER_BYTECODE CreateDomainShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
	};
}


