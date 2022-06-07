#include "stdafx.h"
#include "client/renderer/shader/core/compute_shader.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/renderer/rootsignature/compute_super_root_signature.h"

namespace client_fw
{
	ComputeShader::ComputeShader(const std::string& name)
		: Shader(name)
	{
	}

	D3D12_SHADER_BYTECODE ComputeShader::CreateComputeShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return D3D12_SHADER_BYTECODE{ nullptr, 0 };
	}

	bool ComputeShader::CreatePipelineState(ID3D12Device* device, const SPtr<ComputeRenderLevel>& render_level, int num_of_pso)
	{
		eRenderLevelType render_level_type = render_level->GetRenderLevelType();
		m_pipeline_states[render_level_type].resize(num_of_pso);

		ComPtr<ID3DBlob> compute_blob;
		D3D12_COMPUTE_PIPELINE_STATE_DESC pso_desc;
		ZeroMemory(&pso_desc, sizeof(D3D12_COMPUTE_PIPELINE_STATE_DESC));
		pso_desc.pRootSignature = render_level->GetRootSignature()->GetRootSignature();
		pso_desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

		for (int i = 0; i < num_of_pso; ++i)
		{
			pso_desc.CS = CreateComputeShader(compute_blob.GetAddressOf(), render_level_type, i);

			if (FAILED(device->CreateComputePipelineState(&pso_desc, IID_PPV_ARGS(&m_pipeline_states[render_level_type][i]))))
			{
				DebugPipelineState(render_level_type, i);
				return false;
			}
		}

		return true;
	}

	bool ComputeShader::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		LOG_WARN("Could not supported camera component at {0}", m_name);
		return false;
	}

	void ComputeShader::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		LOG_WARN("Could not supported camera component at {0}", m_name);
	}
}