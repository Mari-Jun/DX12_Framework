#include "stdafx.h"
#include "client/renderer/shader/material_billboard_shader.h"
#include "client/renderer/core/render.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/renderer/renderitem/billboard_render_item.h"

namespace client_fw
{
	MaterialBillboardShader::MaterialBillboardShader(const std::string& name)
		: BillboardShader(name)
	{
		m_billboard_render_item = CreateSPtr<MaterialBillboardRenderItem>(name);
	}

	void MaterialBillboardShader::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kOpaque:
			m_billboard_render_item->Update(device, level_type);
			break;
		default:
			break;
		}
	}

	void MaterialBillboardShader::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		m_billboard_render_item->UpdateFrameResource(device, level_type);
	}

	void MaterialBillboardShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kOpaque:
			m_billboard_render_item->Draw(command_list, level_type,
				[this, command_list, level_type]() {
				command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
				},
				[this, command_list, level_type]() {
					command_list->SetPipelineState(m_pipeline_states.at(level_type)[1].Get());
				});
			break;
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE MaterialBillboardShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "VSMaterialBillboard", "vs_5_1", shader_blob);
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> MaterialBillboardShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(3);

		input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[1] = { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[2] = { "RSINDEX", 0, DXGI_FORMAT_R32_UINT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		return input_element_descs;
	}

	bool MaterialBillboardShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
	{
		bool result = true;

		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kOpaque:
			result &= CreatePipelineState(device, render_level, 2);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}",
				Render::ConvertRenderLevelType(render_level->GetRenderLevelType()), m_name);
			return false;
		}

		return result;
	}

	MaterialBaseColorBillboardShader::MaterialBaseColorBillboardShader(const std::string& name)
		: MaterialBillboardShader(name)
	{
	}

	D3D12_SHADER_BYTECODE MaterialBaseColorBillboardShader::CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		if (pso_index == 0)
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "GSMaterialBaseColorBillboard", "gs_5_1", shader_blob);
		else
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "GSFixUpMaterialBaseColorBillboard", "gs_5_1", shader_blob);
	}

	OpaqueMaterialBaseColorBillboardShader::OpaqueMaterialBaseColorBillboardShader(const std::string& name)
		: MaterialBaseColorBillboardShader(name)
	{
	}

	D3D12_SHADER_BYTECODE OpaqueMaterialBaseColorBillboardShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "PSOpaqueMaterialBaseColorBillboard", "ps_5_1", shader_blob);
	}

	MaskedMaterialBaseColorBillboardShader::MaskedMaterialBaseColorBillboardShader(const std::string& name)
		: MaterialBaseColorBillboardShader(name)
	{
	}

	D3D12_SHADER_BYTECODE MaskedMaterialBaseColorBillboardShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "PSMaskedMaterialBaseColorBillboard", "ps_5_1", shader_blob);
	}

	MaterialTextureBillboardShader::MaterialTextureBillboardShader(const std::string& name)
		: MaterialBillboardShader(name)
	{
	}

	D3D12_SHADER_BYTECODE MaterialTextureBillboardShader::CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		if (pso_index == 0)
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "GSMaterialTextureBillboard", "gs_5_1", shader_blob);
		else
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "GSFixUpMaterialTextureBillboard", "gs_5_1", shader_blob);
	}

	OpaqueMaterialTextureBillboardShader::OpaqueMaterialTextureBillboardShader(const std::string& name)
		: MaterialTextureBillboardShader(name)
	{
	}

	D3D12_SHADER_BYTECODE OpaqueMaterialTextureBillboardShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "PSOpaqueMaterialTextureBillboard", "ps_5_1", shader_blob);
	}

	MaskedMaterialTextureBillboardShader::MaskedMaterialTextureBillboardShader(const std::string& name)
		: MaterialTextureBillboardShader(name)
	{
	}

	D3D12_SHADER_BYTECODE MaskedMaterialTextureBillboardShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "PSMaskedMaterialTextureBillboard", "ps_5_1", shader_blob);
	}

	MaterialNormalMapBillboardShader::MaterialNormalMapBillboardShader(const std::string& name)
		: MaterialBillboardShader(name)
	{
	}

	D3D12_SHADER_BYTECODE MaterialNormalMapBillboardShader::CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		if (pso_index == 0)
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "GSMaterialNormalMapBillboard", "gs_5_1", shader_blob);
		else
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "GSFixUpMaterialNormalMapBillboard", "gs_5_1", shader_blob);
	}

	OpaqueMaterialNormalMapBillboardShader::OpaqueMaterialNormalMapBillboardShader(const std::string& name)
		: MaterialNormalMapBillboardShader(name)
	{
	}

	D3D12_SHADER_BYTECODE OpaqueMaterialNormalMapBillboardShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "PSOpaqueMaterialNormalMapBillboard", "ps_5_1", shader_blob);
	}

	MaskedMaterialNormalMapBillboardShader::MaskedMaterialNormalMapBillboardShader(const std::string& name)
		: MaterialNormalMapBillboardShader(name)
	{
	}

	D3D12_SHADER_BYTECODE MaskedMaterialNormalMapBillboardShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Billboard.hlsl", "PSMaskedMaterialNormalMapBillboard", "ps_5_1", shader_blob);
	}
}
