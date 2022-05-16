#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/shader/opaque_mesh_shader.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/renderer/renderitem/mesh_render_item.h"

namespace client_fw
{
	OpaqueMeshShader::OpaqueMeshShader(const std::string& name)
		: MeshShader(name)
	{
		m_render_item = CreateSPtr<StaticMeshRenderItem>(name);
	}

	void OpaqueMeshShader::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
		case eRenderLevelType::kShadow:
		case eRenderLevelType::kShadowCube:
		case eRenderLevelType::kShadowCascade:
			m_render_item->Update(device, level_type);
			break;
		default:
			break;
		}
	}

	void OpaqueMeshShader::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		m_render_item->UpdateFrameResource(device, level_type);
		//UpdateRenderItemResource(device);
	}

	void OpaqueMeshShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
		case eRenderLevelType::kShadow:
		case eRenderLevelType::kShadowCube:
		case eRenderLevelType::kShadowCascade:
			m_render_item->Draw(command_list, level_type, 
				[this, command_list, level_type]() {
					command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
				});
			break;
		default:
			break;
		}
	}

	D3D12_SHADER_BYTECODE OpaqueMeshShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kShadow:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "VSOpaqueMeshForShadow", "vs_5_1", shader_blob);
		case eRenderLevelType::kShadowCube:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "VSOpaqueMeshForShadowCube", "vs_5_1", shader_blob);
		case eRenderLevelType::kShadowCascade:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "VSOpaqueMeshForShadowCascade", "vs_5_1", shader_blob);
		default:
			return D3D12_SHADER_BYTECODE();
		}
	}

	D3D12_SHADER_BYTECODE OpaqueMeshShader::CreateGeometryShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kShadowCube:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "GSOpaqueMeshForShadowCube", "gs_5_1", shader_blob);
		case eRenderLevelType::kShadowCascade:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "GSOpaqueMeshForShadowCascade", "gs_5_1", shader_blob);
		default:
			return MeshShader::CreateGeometryShader(shader_blob, level_type, pso_index);
		}
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> OpaqueMeshShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		if (level_type == eRenderLevelType::kShadow ||
			level_type == eRenderLevelType::kShadowCube ||
			level_type == eRenderLevelType::kShadowCascade)
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(1);

			input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			return input_element_descs;
		}

		return GraphicsShader::CreateInputLayout(level_type, pso_index);
	}

	D3D12_RASTERIZER_DESC OpaqueMeshShader::CreateRasterizerState(eRenderLevelType level_type, int pso_index) const
	{
		D3D12_RASTERIZER_DESC desc = GraphicsShader::CreateRasterizerState(level_type, pso_index);

		switch (level_type)
		{
		case eRenderLevelType::kShadow:
		case eRenderLevelType::kShadowCube:
			desc.DepthBias = 10000;
			desc.DepthBiasClamp = 0.0f;
			desc.SlopeScaledDepthBias = 20.0f;
			break;
		case eRenderLevelType::kShadowCascade:
			desc.DepthBias = 2500;
			desc.DepthBiasClamp = 0.0f;
			desc.SlopeScaledDepthBias = 1.0f;
			break;
		default:
			break;
		}

		return desc;
	}

	bool OpaqueMeshShader::CreatePipelineStates(ID3D12Device* device, const SPtr<GraphicsRenderLevel>& render_level)
	{
		bool result = true;

		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kOpaque:
		case eRenderLevelType::kShadow:
		case eRenderLevelType::kShadowCube:
		case eRenderLevelType::kShadowCascade:
			result &= CreatePipelineState(device, render_level, 1);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}",
				Render::ConvertRenderLevelType(render_level->GetRenderLevelType()), m_name);
			return false;
		}

		return result;
	}

	OpaqueMaterialMeshShader::OpaqueMaterialMeshShader(const std::string& name)
		: OpaqueMeshShader(name)
	{
	}

	D3D12_SHADER_BYTECODE OpaqueMaterialMeshShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "VSOpaqueMaterialMesh", "vs_5_1", shader_blob);
		default:
			return OpaqueMeshShader::CreateVertexShader(shader_blob, level_type, pso_index);
		}
	}

	D3D12_SHADER_BYTECODE OpaqueMaterialMeshShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "PSOpaqueMaterialMesh", "ps_5_1", shader_blob);
		default:
			return GraphicsShader::CreatePixelShader(shader_blob, level_type, pso_index);
		}
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> OpaqueMaterialMeshShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(2);

			input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			return input_element_descs;
		}
		case eRenderLevelType::kShadow:
		case eRenderLevelType::kShadowCube:
		case eRenderLevelType::kShadowCascade:
		default:
			return OpaqueMeshShader::CreateInputLayout(level_type, pso_index);
		}
	}

	OpaqueTextureMeshShader::OpaqueTextureMeshShader(const std::string& name)
		: OpaqueMeshShader(name)
	{
	}

	D3D12_SHADER_BYTECODE OpaqueTextureMeshShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "VSOpaqueTextureMesh", "vs_5_1", shader_blob);
		default:
			return OpaqueMeshShader::CreateVertexShader(shader_blob, level_type, pso_index);
		}
	}

	D3D12_SHADER_BYTECODE OpaqueTextureMeshShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (level_type)
		{
		case client_fw::eRenderLevelType::kOpaque:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "PSOpaqueTextureMesh", "ps_5_1", shader_blob);
		default:
			return GraphicsShader::CreatePixelShader(shader_blob, level_type, pso_index);
		}
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> OpaqueTextureMeshShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(3);

			input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			return input_element_descs;
		}
		case eRenderLevelType::kShadow:
		case eRenderLevelType::kShadowCube:
		case eRenderLevelType::kShadowCascade:
		default:
			return OpaqueMeshShader::CreateInputLayout(level_type, pso_index);
		}
	}

	OpaqueNormalMapMeshShader::OpaqueNormalMapMeshShader(const std::string& name)
		: OpaqueMeshShader(name)
	{
	}

	D3D12_SHADER_BYTECODE OpaqueNormalMapMeshShader::CreateVertexShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "VSOpaqueNormalMapMesh", "vs_5_1", shader_blob);
		default:
			return OpaqueMeshShader::CreateVertexShader(shader_blob, level_type, pso_index);
		}
	}

	D3D12_SHADER_BYTECODE OpaqueNormalMapMeshShader::CreatePixelShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/Opaque.hlsl", "PSOpaqueNormalMapMesh", "ps_5_1", shader_blob);
		default:
			return GraphicsShader::CreatePixelShader(shader_blob, level_type, pso_index);
		}
	}

	std::vector<D3D12_INPUT_ELEMENT_DESC> OpaqueNormalMapMeshShader::CreateInputLayout(eRenderLevelType level_type, int pso_index) const
	{
		switch (level_type)
		{
		case eRenderLevelType::kOpaque:
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> input_element_descs(5);

			input_element_descs[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[1] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[2] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[3] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
			input_element_descs[4] = { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };

			return input_element_descs;
		}
		case eRenderLevelType::kShadow:
		case eRenderLevelType::kShadowCube:
		case eRenderLevelType::kShadowCascade:
		default:
			return OpaqueMeshShader::CreateInputLayout(level_type, pso_index);
		}
	}
}
