#include "stdafx.h"
#include "client/renderer/shader/sky_shader.h"
#include "client/renderer/renderitem/sky_render_item.h"
#include "client/renderer/core/render.h"
#include "client/renderer/renderlevel/core/render_level.h"

namespace client_fw
{
	SkyShader::SkyShader(const std::string& name)
		: GraphicsShader(name)
	{
		m_sky_render_item = CreateSPtr<SkyRenderItem>(name);
	}

	void SkyShader::Initialize(ID3D12Device* device)
	{
		m_sky_render_item->Initialize(device, m_registered_render_levels);
	}

	void SkyShader::Shutdown()
	{
		m_sky_render_item->Shutdown();
	}

	void SkyShader::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kOpaque:
			m_sky_render_item->Update(device, level_type);
			break;
		default:
			break;
		}
	}

	void SkyShader::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		m_sky_render_item->UpdateFrameResource(device, level_type);
	}

	void SkyShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kOpaque:
			m_sky_render_item->Draw(command_list, level_type,
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

	D3D12_SHADER_BYTECODE SkyShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		if(pso_index == 0)
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Sky.hlsl", "VSSkyCube", "vs_5_1", shader_blob);
		else
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Sky.hlsl", "VSSkySphere", "vs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE SkyShader::CreateHullShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		if (pso_index == 0)
			return GraphicsShader::CreateHullShader(shader_blob, level_type, pso_index);
		else
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Sky.hlsl", "HSSkySphere", "hs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE SkyShader::CreateDomainShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		if (pso_index == 0)
			return GraphicsShader::CreateDomainShader(shader_blob, level_type, pso_index);
		else
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Sky.hlsl", "DSSkySphere", "ds_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE SkyShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		if (pso_index == 0)
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Sky.hlsl", "PSSkyCube", "ps_5_1", shader_blob);
		else
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Sky.hlsl", "PSSkySphere", "ps_5_1", shader_blob);
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> SkyShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		if (pso_index == 0)
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(1);

			input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			return input_element_descs;
		}
		else
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(2);

			input_element_descs[0] = { "CENTERCOLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[1] = { "APEXCOLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			return input_element_descs;
		}
	
	}

	D3D12_RASTERIZER_DESC SkyShader::CreateRasterizerState(eRenderLevelType level_type, int pso_index) const
	{
		D3D12_RASTERIZER_DESC desc = GraphicsShader::CreateRasterizerState(level_type, pso_index);
		desc.CullMode = D3D12_CULL_MODE_FRONT;
		return desc;
	}

	D3D12_DEPTH_STENCIL_DESC SkyShader::CreateDepthStencilState(eRenderLevelType level_type, int pso_index) const
	{
		D3D12_DEPTH_STENCIL_DESC desc = GraphicsShader::CreateDepthStencilState(level_type, pso_index);
		desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		return desc;
	}

	D3D12_PRIMITIVE_TOPOLOGY_TYPE SkyShader::GetPrimitiveTopologyType(eRenderLevelType level_type, int pso_index) const
	{
		if (pso_index == 0)
			return GraphicsShader::GetPrimitiveTopologyType(level_type, pso_index);
		else
			return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
	}

	bool SkyShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
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

	bool SkyShader::RegisterSkyComponent(ID3D12Device* device, const SPtr<SkyComponent>& sky_comp)
	{
		m_sky_render_item->RegisterSkyComponent(sky_comp);
		return true;
	}

	void SkyShader::UnregisterSkyComponent(const SPtr<SkyComponent>& sky_comp)
	{
		m_sky_render_item->UnregisterSkyComponent(sky_comp);
	}
}
