#pragma once
#include "client/renderer/shader/core/graphics_shader.h"

namespace client_fw
{
	class WidgetRenderItem;

	class WidgetShader : public GraphicsShader
	{
	protected:
		WidgetShader(const std::string& name);
		virtual ~WidgetShader() = default;

		virtual void Initialize(ID3D12Device* device) override;
		virtual void Shutdown() override;

		virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_SHADER_BYTECODE CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		
		virtual std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputLayout(eRenderLevelType level_type, int pso_index) const override;

		virtual D3D12_PRIMITIVE_TOPOLOGY_TYPE GetPrimitiveTopologyType(eRenderLevelType level_type, int pso_index) const override;
		virtual bool CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level) override;

		virtual bool RegisterWidgetComponent(ID3D12Device* device, const SPtr<WidgetComponent>& widget_comp);
		virtual void UnregisterWidgetComponent(const SPtr<WidgetComponent>& widget_comp);

	protected:
		SPtr<WidgetRenderItem> m_widget_render_item;
	};

	class OpaqueWidgetShader : public WidgetShader
	{
	public:
		OpaqueWidgetShader(const std::string& name);
		virtual ~OpaqueWidgetShader() = default;

	public:
		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const override;

		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
	};

	class MaskedWidgetShader : public WidgetShader
	{
	public:
		MaskedWidgetShader(const std::string& name);
		virtual ~MaskedWidgetShader() = default;

	public:
		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const override;

		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
	};
}


