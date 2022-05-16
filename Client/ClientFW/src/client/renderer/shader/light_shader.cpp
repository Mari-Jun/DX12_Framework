#include "stdafx.h"
#include "client/renderer/shader/light_shader.h"
#include "client/renderer/core/render.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/renderer/renderitem/light_render_item.h"

namespace client_fw
{
	LightShader::LightShader(const std::string& name)
		: GraphicsShader(name)
	{
	}

	void LightShader::Initialize(ID3D12Device* device)
	{
		m_light_render_item->Initialize(device, m_registered_render_levels);
	}

	void LightShader::Shutdown()
	{
		m_light_render_item->Shutdown();
	}

	D3D12_SHADER_BYTECODE LightShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Deferred.hlsl", "VSLocalLight", "vs_5_1", shader_blob);
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> LightShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(1);

		input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		return input_element_descs;
	}

	D3D12_RASTERIZER_DESC LightShader::CreateRasterizerState(eRenderLevelType level_type, int pso_index) const
	{
		D3D12_RASTERIZER_DESC desc = GraphicsShader::CreateRasterizerState(level_type, pso_index);
		desc.CullMode = D3D12_CULL_MODE_FRONT;
		desc.DepthClipEnable = FALSE;
		return desc;
	}

	D3D12_BLEND_DESC LightShader::CreateBlendState(eRenderLevelType level_type, int pso_index) const
	{
		D3D12_BLEND_DESC desc = GraphicsShader::CreateBlendState(level_type, pso_index);
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
		desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
		return desc;
	}

	D3D12_DEPTH_STENCIL_DESC LightShader::CreateDepthStencilState(eRenderLevelType level_type, int pso_index) const
	{
		D3D12_DEPTH_STENCIL_DESC desc = GraphicsShader::CreateDepthStencilState(level_type, pso_index);
		desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		return desc;
	}

	D3D12_PRIMITIVE_TOPOLOGY_TYPE LightShader::GetPrimitiveTopologyType(eRenderLevelType level_type, int pso_index) const
	{
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
	}

	bool LightShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
	{
		bool result = true;

		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kDeferred:
			result &= CreatePipelineState(device, render_level, 1);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}",
				Render::ConvertRenderLevelType(render_level->GetRenderLevelType()), m_name);
			return false;
		}

		return result;
	}

	bool LightShader::RegisterLocalLightComponent(ID3D12Device* device, const SPtr<LocalLightComponent> light_comp)
	{
		m_light_render_item->RegisterLightComponent(light_comp);
		return true;
	}

	void LightShader::UnregisterLocalLightComponent(const SPtr<LocalLightComponent>& light_comp)
	{
		m_light_render_item->UnregisterLightComponent(light_comp);
	}

	PointLightShader::PointLightShader(const std::string& name)
		: LightShader(name)
	{
		m_light_render_item = CreateSPtr<PointLightRenderItem>(name);
	}

	void PointLightShader::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kDeferred:
			m_light_render_item->Update(device, level_type);
			break;
		default:
			break;
		}
	}

	void PointLightShader::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		m_light_render_item->UpdateFrameResource(device, level_type);
	}

	void PointLightShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kDeferred:
		{
			m_light_render_item->Draw(command_list, level_type,
				[this, command_list, level_type]() {
					
					command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
				});
			break;
		}
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE PointLightShader::CreateHullShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Deferred.hlsl", "HSPointLight", "hs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE PointLightShader::CreateDomainShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Deferred.hlsl", "DSPointLight", "ds_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE PointLightShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Deferred.hlsl", "PSPointLight", "ps_5_1", shader_blob);
	}

	SpotLightShader::SpotLightShader(const std::string& name)
		: LightShader(name)
	{
		m_light_render_item = CreateSPtr<SpotLightRenderItem>(name);
	}

	void SpotLightShader::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kDeferred:
		{
			m_light_render_item->Update(device, level_type);
			break;
		}
		default:
			break;
		}
	}

	void SpotLightShader::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		m_light_render_item->UpdateFrameResource(device, level_type);
	}

	void SpotLightShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kDeferred:
		{
			m_light_render_item->Draw(command_list, level_type,
				[this, command_list, level_type]() {

					command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
				});
			break;
		}
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE SpotLightShader::CreateHullShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Deferred.hlsl", "HSSpotLight", "hs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE SpotLightShader::CreateDomainShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Deferred.hlsl", "DSSpotLight", "ds_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE SpotLightShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Deferred.hlsl", "PSSpotLight", "ps_5_1", shader_blob);
	}
}
