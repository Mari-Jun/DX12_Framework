#include "stdafx.h"
#include "client/renderer/shader/widget_shader.h"
#include "client/renderer/renderitem/widget_render_item.h"
#include "client/renderer/core/render.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/object/component/render/widget_component.h"

namespace client_fw
{
	WidgetShader::WidgetShader(const std::string& name)
		: GraphicsShader(name)
	{
		m_widget_render_item = CreateSPtr<WidgetRenderItem>(name);
	}

	void WidgetShader::Initialize(ID3D12Device* device)
	{
		m_widget_render_item->Initialize(device, m_registered_render_levels);
	}

	void WidgetShader::Shutdown()
	{
		m_widget_render_item->Shutdown();
	}

	D3D12_SHADER_BYTECODE WidgetShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		if (pso_index == 0)
		{
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Widget.hlsl", "VSWorldWidget", "vs_5_1", shader_blob);
		}
		else
		{
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Widget.hlsl", "VSPivotWidget", "vs_5_1", shader_blob);
		}

	}

	D3D12_SHADER_BYTECODE WidgetShader::CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (pso_index)
		{
		case 1:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Widget.hlsl", "GSBillboardWidget", "gs_5_1", shader_blob);
		case 2:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Widget.hlsl", "GSFixUpBillboardWidget", "gs_5_1", shader_blob);
		case 0:
		default:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Widget.hlsl", "GSWorldWidget", "gs_5_1", shader_blob);
		}
	
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> WidgetShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		if (pso_index == 0)
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(8);

			input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[1] = { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[2] = { "TEXINDEX", 0, DXGI_FORMAT_R32_SINT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[3] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[4] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[5] = { "TILLING", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[6] = { "RIGHT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[7] = { "UP", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 68, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			return input_element_descs;
		}
		else
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(7);

			input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[1] = { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[2] = { "TEXINDEX", 0, DXGI_FORMAT_R32_SINT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[3] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[4] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[5] = { "TILLING", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[6] = { "PIVOT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			return input_element_descs;
		}
	
	}

	D3D12_PRIMITIVE_TOPOLOGY_TYPE WidgetShader::GetPrimitiveTopologyType(eRenderLevelType level_type, int pso_index) const
	{
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	}

	bool WidgetShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
	{
		bool result = true;

		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kOpaque:
			result &= CreatePipelineState(device, render_level, 3);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}",
				Render::ConvertRenderLevelType(render_level->GetRenderLevelType()), m_name);
			return false;
		}

		return result;
	}

	bool WidgetShader::RegisterWidgetComponent(ID3D12Device* device, const SPtr<WidgetComponent>& widget_comp)
	{
		m_widget_render_item->RegisterWidgetComponent(widget_comp);
		return true;
	}

	void WidgetShader::UnregisterWidgetComponent(const SPtr<WidgetComponent>& widget_comp)
	{
		m_widget_render_item->UnregisterWidgetComponent(widget_comp);
	}

	OpaqueWidgetShader::OpaqueWidgetShader(const std::string& name)
		: WidgetShader(name)
	{
	}

	void OpaqueWidgetShader::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kOpaque:
		{
			m_widget_render_item->Update(device, level_type);
			break;
		}
		default:
			break;
		}
	}

	void OpaqueWidgetShader::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		m_widget_render_item->UpdateFrameResource(device, level_type);
	}

	void OpaqueWidgetShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kOpaque:
		{
			m_widget_render_item->Draw(command_list, level_type,
				[this, command_list, level_type]() {
					command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
				},
				[this, command_list, level_type]() {
					command_list->SetPipelineState(m_pipeline_states.at(level_type)[1].Get());
				},
					[this, command_list, level_type]() {
					command_list->SetPipelineState(m_pipeline_states.at(level_type)[2].Get());
				});
			break;
		}
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE OpaqueWidgetShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Widget.hlsl", "PSOpaqueWidget", "ps_5_1", shader_blob);
	}

	MaskedWidgetShader::MaskedWidgetShader(const std::string& name)
		: WidgetShader(name)
	{
	}

	void MaskedWidgetShader::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kOpaque:
		{
			m_widget_render_item->Update(device, level_type);
			break;
		}
		default:
			break;
		}
	}

	void MaskedWidgetShader::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		m_widget_render_item->UpdateFrameResource(device, level_type);
	}

	void MaskedWidgetShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kOpaque:
		{
			m_widget_render_item->Draw(command_list, level_type,
				[this, command_list, level_type]() {
					command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
				},
				[this, command_list, level_type]() {
					command_list->SetPipelineState(m_pipeline_states.at(level_type)[1].Get());
				},
				[this, command_list, level_type]() {
					command_list->SetPipelineState(m_pipeline_states.at(level_type)[2].Get());
				});
			break;
		}
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE MaskedWidgetShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Widget.hlsl", "PSMaskedWidget", "ps_5_1", shader_blob);
	}

}
