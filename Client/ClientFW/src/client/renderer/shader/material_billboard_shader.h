#pragma once
#include "client/renderer/shader/core/graphics_shader.h"

namespace client_fw
{
	class MaterialBillboardShader : public BillboardShader
	{
	protected:
		MaterialBillboardShader(const std::string& name);
		virtual ~MaterialBillboardShader() = default;

	public:
		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const override;

		virtual D3D12_SHADER_BYTECODE CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;

		virtual std::vector<D3D12_INPUT_ELEMENT_DESC> CreateInputLayout(eRenderLevelType level_type, int pso_index) const override;

		virtual bool CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level) override;
	};

	//
	// Material Base Color
	//

	class MaterialBaseColorBillboardShader : public MaterialBillboardShader
	{
	protected:
		MaterialBaseColorBillboardShader(const std::string& name);
		virtual ~MaterialBaseColorBillboardShader() = default;
		
	public:
		virtual D3D12_SHADER_BYTECODE CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
	};

	class OpaqueMaterialBaseColorBillboardShader : public MaterialBaseColorBillboardShader
	{
	public:
		OpaqueMaterialBaseColorBillboardShader(const std::string& name);
		virtual ~OpaqueMaterialBaseColorBillboardShader() = default;

		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
	};

	class MaskedMaterialBaseColorBillboardShader : public MaterialBaseColorBillboardShader
	{
	public:
		MaskedMaterialBaseColorBillboardShader(const std::string& name);
		virtual ~MaskedMaterialBaseColorBillboardShader() = default;

		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
	};

	//
	// Material Texture (Diffuse)
	//

	class MaterialTextureBillboardShader : public MaterialBillboardShader
	{
	protected:
		MaterialTextureBillboardShader(const std::string& name);
		virtual ~MaterialTextureBillboardShader() = default;

	public:
		virtual D3D12_SHADER_BYTECODE CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
	};

	class OpaqueMaterialTextureBillboardShader : public MaterialTextureBillboardShader
	{
	public:
		OpaqueMaterialTextureBillboardShader(const std::string& name);
		virtual ~OpaqueMaterialTextureBillboardShader() = default;

		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
	};

	class MaskedMaterialTextureBillboardShader : public MaterialTextureBillboardShader
	{
	public:
		MaskedMaterialTextureBillboardShader(const std::string& name);
		virtual ~MaskedMaterialTextureBillboardShader() = default;

		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
	};

	//
	// Material Normal Map 
	//

	class MaterialNormalMapBillboardShader : public MaterialBillboardShader
	{
	protected:
		MaterialNormalMapBillboardShader(const std::string& name);
		virtual ~MaterialNormalMapBillboardShader() = default;

	public:
		virtual D3D12_SHADER_BYTECODE CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
	};

	class OpaqueMaterialNormalMapBillboardShader : public MaterialNormalMapBillboardShader
	{
	public:
		OpaqueMaterialNormalMapBillboardShader(const std::string& name);
		virtual ~OpaqueMaterialNormalMapBillboardShader() = default;

		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
	};

	class MaskedMaterialNormalMapBillboardShader : public MaterialNormalMapBillboardShader
	{
	public:
		MaskedMaterialNormalMapBillboardShader(const std::string& name);
		virtual ~MaskedMaterialNormalMapBillboardShader() = default;

		virtual D3D12_SHADER_BYTECODE CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;
	};
}