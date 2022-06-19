#include "stdafx.h"
#include "client/renderer/core/render.h"
#include "client/renderer/shader/render_camera_post_processing_shader.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/render_camera_post_processing_frame_resource.h"
#include "client/object/component/util/render_camera_component.h"
#include "client/asset/texture/texture.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	RenderCameraPostProcessingShader::RenderCameraPostProcessingShader(const std::string& name)
		: ComputeShader(name)
	{
	}

	void RenderCameraPostProcessingShader::Initialize(ID3D12Device* device)
	{
		const auto& frame_resources = FrameResourceManager::GetManager().GetFrameResources();
		for (const auto& frame : frame_resources)
		{
			for (eRenderLevelType level_type : m_registered_render_levels)
				frame->CreateRenderCameraPostProcessingFrameResource(device, m_name, level_type);
		}
	}

	void RenderCameraPostProcessingShader::Shutdown()
	{
	}

	void RenderCameraPostProcessingShader::Update(ID3D12Device* device, eRenderLevelType level_type)
	{
	}

	void RenderCameraPostProcessingShader::UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type)
	{
		const auto& frame_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->
			GetRenderCameraPostProcessingFrameResource(m_name, level_type);

		UINT post_processing_size = frame_resource->GetSizeOfPostProcessing();
		UINT new_size = static_cast<UINT>(m_render_cameras.size());
		bool is_need_resource_create = false;

		while (post_processing_size <= new_size)
		{
			post_processing_size = static_cast<UINT>(roundf(static_cast<float>(post_processing_size) * 1.5f));
			is_need_resource_create = true;
		}

		if (is_need_resource_create)
		{
			frame_resource->GetRenderCameraPostProcessingData()->CreateResource(device, post_processing_size);
			frame_resource->SetSizeOfPostProcessing(post_processing_size);
		}

		RSRenderCameraPostProcessingData post_processing_data;
		UINT index = 0;

		for (const auto& camera : m_render_cameras)
		{
			if (camera->GetCameraState() == eCameraState::kActive)
			{
				const auto& post_procssing_info = camera->GetPostProcessingInfo();

				if (post_procssing_info->use_blur)
				{
					BlurInfo info = GetBlurInfo(post_procssing_info->blur_sigma);
					post_processing_data.blur_radius = info.radius;
					for (int i = 0; i < min(post_processing_data.blur_weights.size(), info.weights.size()); ++i)
						post_processing_data.blur_weights[i] = info.weights[i];
				}
			

				frame_resource->GetRenderCameraPostProcessingData()->CopyData(index, post_processing_data);
			}

			++index;
		}
	}

	void RenderCameraPostProcessingShader::Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const
	{
		const auto& frame_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->
			GetRenderCameraPostProcessingFrameResource(m_name, level_type);

		UINT index = 0;

		for (const auto& camera : m_render_cameras)
		{
			D3D12_GPU_VIRTUAL_ADDRESS gpu_address;

			if (camera->GetCameraState() == eCameraState::kActive)
			{
				gpu_address = frame_resource->GetRenderCameraPostProcessingData()->GetResource()->GetGPUVirtualAddress() +
					index * frame_resource->GetRenderCameraPostProcessingData()->GetByteSize();

				command_list->SetComputeRootConstantBufferView(2, gpu_address);

				DrawSobelEdge(command_list, level_type, camera);
				DrawBlur(command_list, level_type, camera);
			}

			++index;
		}
	}

	void RenderCameraPostProcessingShader::DrawSobelEdge(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type, 
		const SPtr<RenderCameraComponent>& camera) const
	{
		const auto& render_texture = camera->GetRenderTexture();
		const auto& blur_texture1 = camera->GetRWTexture("pp texture 1");

		if (blur_texture1->GetResource() != nullptr)
		{
			const auto& post_procssing_info = camera->GetPostProcessingInfo();

			if (post_procssing_info->use_sobel_edge)
			{
				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture1->GetResource(),
					D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

				command_list->SetPipelineState(m_pipeline_states.at(level_type)[2].Get());
				command_list->SetComputeRootDescriptorTable(0, render_texture->GetGPUHandle());
				command_list->SetComputeRootDescriptorTable(1, blur_texture1->GetGPUHandle());

				UINT num_group_x = static_cast<UINT>(ceilf(static_cast<float>(blur_texture1->GetTextureSize().x) / 16.0f));
				UINT num_group_y = static_cast<UINT>(ceilf(static_cast<float>(blur_texture1->GetTextureSize().y) / 16.0f));

				command_list->Dispatch(num_group_x, num_group_y, 1);

				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(render_texture->GetResource(),
					D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_DEST));
				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture1->GetResource(),
					D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE));

				command_list->CopyResource(render_texture->GetResource(), blur_texture1->GetResource());

				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(render_texture->GetResource(),
					D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture1->GetResource(),
					D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COMMON));
			}
		}
	}

	void RenderCameraPostProcessingShader::DrawBlur(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type,
		const SPtr<RenderCameraComponent>& camera) const
	{
		const auto& render_texture = camera->GetRenderTexture();
		const auto& blur_texture1 = camera->GetRWTexture("pp texture 1");
		const auto& blur_texture2 = camera->GetRWTexture("pp texture 2");

		if (blur_texture1->GetResource() != nullptr && blur_texture2->GetResource() != nullptr)
		{
			const auto& post_procssing_info = camera->GetPostProcessingInfo();

			if (post_procssing_info->use_blur)
			{
				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(render_texture->GetResource(),
					D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_SOURCE));
				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture1->GetResource(),
					D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));

				command_list->CopyResource(blur_texture1->GetResource(), render_texture->GetResource());

				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture1->GetResource(),
					D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture2->GetResource(),
					D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));

				for (int i = 0; i < post_procssing_info->blur_count; ++i)
				{
					command_list->SetPipelineState(m_pipeline_states.at(level_type)[0].Get());
					command_list->SetComputeRootDescriptorTable(0, blur_texture1->GetGPUHandle());
					command_list->SetComputeRootDescriptorTable(1, blur_texture2->GetGPUHandle());

					UINT num_group_x = static_cast<UINT>(ceilf(static_cast<float>(blur_texture1->GetTextureSize().x) / 256.0f));
					command_list->Dispatch(num_group_x, blur_texture1->GetTextureSize().y, 1);

					command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture1->GetResource(),
						D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));
					command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture2->GetResource(),
						D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ));

					command_list->SetPipelineState(m_pipeline_states.at(level_type)[1].Get());
					command_list->SetComputeRootDescriptorTable(1, blur_texture1->GetGPUHandle());
					command_list->SetComputeRootDescriptorTable(0, blur_texture2->GetGPUHandle());

					UINT num_group_y = static_cast<UINT>(ceilf(static_cast<float>(blur_texture1->GetTextureSize().y) / 256.0f));
					command_list->Dispatch(blur_texture1->GetTextureSize().x, num_group_y, 1);

					command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture1->GetResource(),
						D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_GENERIC_READ));
					command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture2->GetResource(),
						D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_UNORDERED_ACCESS));
				}

				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(render_texture->GetResource(),
					D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COPY_DEST));
				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture1->GetResource(),
					D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_COPY_SOURCE));

				command_list->CopyResource(render_texture->GetResource(), blur_texture1->GetResource());

				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(render_texture->GetResource(),
					D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture1->GetResource(),
					D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COMMON));
				command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(blur_texture2->GetResource(),
					D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COMMON));
			}
		}
	}

	D3D12_SHADER_BYTECODE RenderCameraPostProcessingShader::CreateComputeShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const
	{
		switch (pso_index)
		{
		case 0:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/RenderCameraPostProcessing.hlsl", "CSHorizontalBlur", "cs_5_1", shader_blob);
		case 1:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/RenderCameraPostProcessing.hlsl", "CSVerticalBlur", "cs_5_1", shader_blob);
		case 2:
			return CompileShader(L"../ClientFW/src/client/renderer/hlsl/RenderCameraPostProcessing.hlsl", "CSSobelEdge", "cs_5_1", shader_blob);
		default:
			return D3D12_SHADER_BYTECODE();
		}
	}

	bool RenderCameraPostProcessingShader::CreatePipelineStates(ID3D12Device* device, const SPtr<ComputeRenderLevel>& render_level)
	{
		bool result = true;

		switch (render_level->GetRenderLevelType())
		{
		case eRenderLevelType::kPostProcess:
			result &= CreatePipelineState(device, render_level, 3);
			break;
		default:
			LOG_ERROR("Could not support {0} from {1}",
				Render::ConvertRenderLevelType(render_level->GetRenderLevelType()), m_name);
			return false;
		}

		return result;
	}

	bool RenderCameraPostProcessingShader::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		const auto& render_camera = std::dynamic_pointer_cast<RenderCameraComponent>(camera_comp);
		if (render_camera != nullptr && render_camera->GetPostProcessingInfo()->use_post_processing)
		{
			m_render_cameras.push_back(render_camera);
			render_camera->SetRWTexture("pp texture 1", CreateSPtr<RWTexture>());
			render_camera->SetRWTexture("pp texture 2", CreateSPtr<RWTexture>());
			return true;
		}
		return false;
	}

	void RenderCameraPostProcessingShader::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
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

	BlurInfo RenderCameraPostProcessingShader::GetBlurInfo(float sigma) const
	{
		BlurInfo info;
		info.radius = min(s_max_blur_radius, static_cast<int>(ceil(2.0f * sigma)));

		sigma = 2.0f * sigma * sigma;

		info.weights.resize(2 * info.radius + 1);

		float sum = 0.0f;
		for (int i = -info.radius; i <= info.radius; ++i)
		{
			info.weights[i + info.radius] = expf(-i * i / sigma);
			sum += info.weights[i + info.radius];
		}

		for (auto& weight : info.weights)
			weight /= sum;

		return info;
	}
}