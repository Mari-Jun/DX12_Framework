#pragma once
#include "client/renderer/shader/core/graphics_shader.h"

namespace client_fw
{
	class OpaqueMeshShader : public MeshShader
	{
	protected:
		OpaqueMeshShader(const std::string& name);
		virtual ~OpaqueMeshShader() = default;

	public:
		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const override;

		virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_SHADER_BYTECODE CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;

		virtual std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputLayout(eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_RASTERIZER_DESC CreateRasterizerState(eRenderLevelType level_type, int pso_index) const override;
		
		virtual bool CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level) override;
	};

	class OpaqueMaterialMeshShader final : public OpaqueMeshShader
	{
	public:
		OpaqueMaterialMeshShader(const std::string& name);
		virtual ~OpaqueMaterialMeshShader() = default;

		virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;

		virtual std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputLayout(eRenderLevelType level_type, int pso_index) const override;
	};

	class OpaqueTextureMeshShader final : public OpaqueMeshShader
	{
	public:
		OpaqueTextureMeshShader(const std::string& name);
		virtual ~OpaqueTextureMeshShader() = default;

		virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;

		virtual std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputLayout(eRenderLevelType level_type, int pso_index) const override;
	};

	class OpaqueNormalMapMeshShader final : public OpaqueMeshShader
	{
	public:
		OpaqueNormalMapMeshShader(const std::string& name);
		virtual ~OpaqueNormalMapMeshShader() = default;

		virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;

		virtual std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputLayout(eRenderLevelType level_type, int pso_index) const override;
	};

}


