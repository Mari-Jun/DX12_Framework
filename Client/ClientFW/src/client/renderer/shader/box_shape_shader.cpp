#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/shader/box_shape_shader.h"
#include "client/renderer/renderlevel/core/render_level.h"

namespace client_fw
{
	BoxShapeShader::BoxShapeShader(const std::string& name)
		: ShapeShader(name)
	{
	}

	void BoxShapeShader::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		UpdateRenderItem(device);
	}

	void BoxShapeShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
			command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
			DrawRenderItem(command_list);
			break;
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE BoxShapeShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Shape.hlsl", "VSShape", "vs_5_1", shader_blob);
	}

	/*D3D12_SHADER_BYTECODE BoxShapeShader::CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pos_index) const
	{
		return D3D12_SHADER_BYTECODE();
	}*/

	D3D12_SHADER_BYTECODE BoxShapeShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Shape.hlsl", "PSShape", "ps_5_1", shader_blob);
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> BoxShapeShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(5);

		input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[1] = { "SIZE", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[2] = { "FORWARD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[3] = { "RIGHT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		input_element_descs[4] = { "UP", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

		return input_element_descs;
	}

	bool BoxShapeShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
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
