#pragma once
#include "client/renderer/shader/core/shader.h"

namespace client_fw
{
	class GraphicsRenderLevel;

	class MeshComponent;
	class ShapeComponent;
	class BillboardComponent;
	class WidgetComponent;
	class SkyComponent;
	class LocalLightComponent;

	class MeshRenderItem;
	class BillboardRenderItem;

	class GraphicsShader : public Shader
	{
	protected:
		GraphicsShader(const std::string& name);
		virtual ~GraphicsShader() = default;

	public:
		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override {}
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override {}
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const override {}

	public:
		virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const = 0;
		virtual D3D12_SHADER_BYTECODE CreateHullShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const;
		virtual D3D12_SHADER_BYTECODE CreateDomainShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const;
		virtual D3D12_SHADER_BYTECODE CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const;
		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const;

		virtual std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputLayout(eRenderLevelType level_type, int pso_index) const;
		virtual D3D12_RASTERIZER_DESC CreateRasterizerState(eRenderLevelType level_type, int pso_index) const;
		virtual D3D12_BLEND_DESC CreateBlendState(eRenderLevelType level_type, int pso_index) const;
		virtual D3D12_DEPTH_STENCIL_DESC CreateDepthStencilState(eRenderLevelType level_type, int pso_index) const;
		virtual D3D12_STREAM_OUTPUT_DESC CreateStreamOutputState(eRenderLevelType level_type, int pso_index) const;

		virtual D3D12_PRIMITIVE_TOPOLOGY_TYPE GetPrimitiveTopologyType(eRenderLevelType level_type, int pso_index) const;

		virtual bool CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level) = 0;

	protected:
		bool CreatePipelineState(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level, int num_of_pso);

	private:
		virtual D3D12_SHADER_BYTECODE CreateShader(ID3DBlob** shader_blob) const;

	public:
		virtual bool RegisterMeshComponent(ID3D12Device* device, const SPtr<MeshComponent>& mesh_comp);
		virtual void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp);
		virtual bool RegisterShapeComponent(ID3D12Device* device, const SPtr<ShapeComponent>& shape_comp);
		virtual void UnregisterShapeComponent(const SPtr<ShapeComponent>& shape_comp);
		virtual bool RegisterBillboardComponent(ID3D12Device* device, const SPtr<BillboardComponent>& bb_comp);
		virtual void UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp);
		virtual bool RegisterWidgetComponent(ID3D12Device* device, const SPtr<WidgetComponent>& widget_comp);
		virtual void UnregisterWidgetComponent(const SPtr<WidgetComponent>& widget_comp);
		virtual bool RegisterSkyComponent(ID3D12Device* device, const SPtr<SkyComponent>& sky_comp);
		virtual void UnregisterSkyComponent(const SPtr<SkyComponent>& sky_comp);
		virtual bool RegisterLocalLightComponent(ID3D12Device* device, const SPtr<LocalLightComponent> light_comp);
		virtual void UnregisterLocalLightComponent(const SPtr<LocalLightComponent>& light_comp);
	};

	class MeshShader : public GraphicsShader
	{
	protected:
		MeshShader(const std::string& name);
		virtual ~MeshShader() = default;

		virtual void Initialize(ID3D12Device* device) override;
		virtual void Shutdown() override;

		virtual bool RegisterMeshComponent(ID3D12Device* device, const SPtr<MeshComponent>& mesh_comp) override final;
		virtual void UnregisterMeshComponent(const SPtr<MeshComponent>& mesh_comp) override final;

	protected:
		SPtr<MeshRenderItem> m_render_item;
	};

	class ShapeShader : public GraphicsShader
	{
	protected:
		ShapeShader(const std::string& name);
		virtual ~ShapeShader() = default;

		virtual void Initialize(ID3D12Device* device) override;
		virtual void Shutdown() override;

		virtual void UpdateRenderItem(ID3D12Device* device);
		virtual void UpdateRenderItemResource(ID3D12Device* device);
		virtual void DrawRenderItem(ID3D12GraphicsCommandList* command_list) const;

		virtual D3D12_RASTERIZER_DESC CreateRasterizerState(eRenderLevelType level_type, int pso_index) const override;

		virtual D3D12_PRIMITIVE_TOPOLOGY_TYPE GetPrimitiveTopologyType(eRenderLevelType level_type, int pso_index) const override;

		virtual bool RegisterShapeComponent(ID3D12Device* device, const SPtr<ShapeComponent>& shape_comp) override final;
		virtual void UnregisterShapeComponent(const SPtr<ShapeComponent>& shape_comp) override final;

	private:

	};

	class BillboardShader : public GraphicsShader
	{
	protected:
		BillboardShader(const std::string& name);
		virtual ~BillboardShader() = default;

		virtual void Initialize(ID3D12Device* device) override;
		virtual void Shutdown() override;

		virtual D3D12_PRIMITIVE_TOPOLOGY_TYPE GetPrimitiveTopologyType(eRenderLevelType level_type, int pso_index) const override;

		virtual bool RegisterBillboardComponent(ID3D12Device* device, const SPtr<BillboardComponent>& bb_comp) override;
		virtual void UnregisterBillboardComponent(const SPtr<BillboardComponent>& bb_comp) override;

	protected:
		SPtr<BillboardRenderItem> m_billboard_render_item;
	};
}


