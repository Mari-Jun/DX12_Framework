#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/shader/test_post_process_shader.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/object/component/util/render_camera_component.h"

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
		LOG_TRACE(m_render_cameras.size());
		for (const auto& camera : m_render_cameras)
		{
			if(camera->GetCameraState() == eCameraState::kActive)
				LOG_INFO(camera->GetName());
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