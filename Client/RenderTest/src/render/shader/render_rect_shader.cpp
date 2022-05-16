#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/renderer/renderlevel/core/render_level.h>
#include <client/renderer/core/render.h>
#include "render/shader/render_rect_shader.h"

namespace render_test
{
	RenderRectShader::RenderRectShader(const std::string& name)
		: MeshShader(name)
	{
	}

	void RenderRectShader::Initialize(ID3D12Device* device)
	{
	}

	void RenderRectShader::Shutdown()
	{
	}

	void RenderRectShader::Update(ID3D12Device* device,eRenderLevelType level_type)
	{
		//UpdateRenderItem(device);
	}

	void RenderRectShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
			command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
			//DrawRenderItem(command_list);
			break;
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE RenderRectShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../RenderTest/src/render/hlsl/render_rect.hlsl", "VSDiffuse", "vs_5_1", shader_blob);
	}

	D3D12_SHADER_BYTECODE RenderRectShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../RenderTest/src/render/hlsl/render_rect.hlsl", "PSDiffuse", "ps_5_1", shader_blob);
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> RenderRectShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(1);
		input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		return input_element_descs;
	}

	bool RenderRectShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
	{
		bool result = true;

		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kOpaque:
			result &= CreatePipelineState(device, render_level, 1);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}",
				Render::ConvertRenderLevelType(render_level->GetRenderLevelType()), m_name);
			return false;
		}

		return result;
	}
}
