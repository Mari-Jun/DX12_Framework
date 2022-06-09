#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/shader/test_post_process_shader.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/object/component/util/render_camera_component.h"
#include "client/asset/texture/texture.h"

namespace client_fw
{
	TestPostProcessShader::TestPostProcessShader(const std::string& name)
		: ComputeShader(name)
	{
	}

	void TestPostProcessShader::Initialize(ID3D12Device* device)
	{
	}

	void TestPostProcessShader::Shutdown()
	{
	}

	void TestPostProcessShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		for (const auto& camera : m_render_cameras)
		{
			if (camera->GetCameraState() == eCameraState::kActive)
			{
				const auto& render_texture = camera->GetRenderTexture();
				const auto& rw_texture = camera->GetRWTexture("test post process");
				if (rw_texture->GetResource() != nullptr)
				{
					command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());

					command_list->SetComputeRootDescriptorTable(0, render_texture->GetGPUHandle());
					command_list->SetComputeRootDescriptorTable(1, rw_texture->GetUAVGPUHandle());				

					command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rw_texture->GetResource(),
						D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

					UINT num_group_x = static_cast<UINT>(rw_texture->GetTextureSize().x / 32 + 1);
					command_list->Dispatch(num_group_x, rw_texture->GetTextureSize().y, 1);

					command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(render_texture->GetResource(),
						D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_DEST));
					command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rw_texture->GetResource(),
						D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE));

					command_list->CopyResource(render_texture->GetResource(), rw_texture->GetResource());

					command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(render_texture->GetResource(),
						D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
					command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(rw_texture->GetResource(),
						D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COMMON));
				}

			}
		}
	}

	D3D12_SHADER_BYTECODE TestPostProcessShader::CreateComputeShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		return CompileShader(L"../ClientFW/src/client/renderer/hlsl/PostProcess.hlsl", "CS", "cs_5_1", shader_blob);
	}

	bool TestPostProcessShader::CreatePipelineStates(ID3D12Device* device, const SPtr<ComputeRenderLevel>& render_level)
	{
		bool result = true;

		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kPostProcess:
			result &= CreatePipelineState(device, render_level, 1);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}",
				Render::ConvertRenderLevelType(render_level->GetRenderLevelType()), m_name);
			return false;
		}

		return result;
	}

	bool TestPostProcessShader::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		const auto& render_camera = std::dynamic_pointer_cast<RenderCameraComponent>(camera_comp);
		if (render_camera != nullptr)
		{
			m_render_cameras.push_back(render_camera);
			render_camera->SetRWTexture("test post process", CreateSPtr<RWTexture>(render_camera->GetViewSize()));
			return true;
		}
		return false;
	}

	void TestPostProcessShader::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		const auto& render_camera = std::dynamic_pointer_cast<RenderCameraComponent>(camera_comp);
		if (render_camera != nullptr)
		{
			auto iter = std::find(m_render_cameras.begin(), m_render_cameras.end(), render_camera);
			if (iter != m_render_cameras.end())
			{
				std::iter_swap(iter, m_render_cameras.end() - 1);
				m_render_cameras.pop_back();
			}
		}
	}
}