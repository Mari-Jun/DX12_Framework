#include "stdafx.h"
#include "client/renderer/core/render_camera_manager.h"
#include "client/renderer/core/light_manager.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/camera_frame_resource.h"
#include "client/renderer/core/mesh_visualizer.h"

#include "client/object/component/util/render_camera_component.h"
#include "client/object/component/util/shadow_camera_component.h"
#include "client/object/component/util/shadow_cube_camera_component.h"
#include "client/object/component/util/shadow_cascade_camera_component.h"
#include "client/object/component/light/directional_light_component.h"

#include "client/asset/texture/texture.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	RenderCameraManager::RenderCameraManager()
	{
	}

	RenderCameraManager::~RenderCameraManager()
	{
	}

	void RenderCameraManager::Shutdown()
	{
	}

	void RenderCameraManager::Update(ID3D12Device* device,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_render_camera)
	{
		ResizeRenderCamerasTexture(device);

		UpdateRenderCameras(device);

		UpdateCameraResource(device,
			std::move(update_shader_function_for_render_camera));
	}

	void RenderCameraManager::ResizeRenderCamerasTexture(ID3D12Device* device)
	{
		int resize_count = 0;

		for (auto i_camera = m_render_cameras.begin(); i_camera != m_render_cameras.end() - resize_count;)
		{
			const auto& camera = *i_camera;
			if (camera->RequireResizeTexture())
			{
				if (camera == m_main_camera)
				{
					m_ready_main_camera = m_main_camera;
					m_main_camera = nullptr;
				}
				m_ready_render_cameras.push_back(camera);
				std::iter_swap(i_camera, m_render_cameras.rbegin() + resize_count);
				++resize_count;
			}
			else
			{
				++i_camera;
			}
		}

		while (resize_count--)
			m_render_cameras.pop_back();
	}

	void RenderCameraManager::UpdateRenderCameras(ID3D12Device* device)
	{
		UpdateRenderCamerasForCascadeShadow(device);

		// ���⼭ wait�� ready camera�� �ִµ� �̿� ���� �ִ� ������ 
		// camera�� ��ϵǸ鼭 camera�� render texture�� �ʱ�ȭ �� �ʿ䰡 �ִµ�
		// �̶� GraphicsCommandList�� ����ϱ� ������ �̸� ����ϱ� ���� �ʱ�ȭ ���� �ʿ䰡 �־�
		// ready camera�� ����Ͽ���,
		// wait ī�޶�� camera render texture�� �ʱ�ȭ �Ǵ� ������ CamaraManger�� Updateȣ�� ��,
		// Draw�Լ� ȣ�� ���̱� ������ Update�� ī�޶��� ���� Draw�� ī�޶��� ���� �ٸ��� ������ ���� ������
		// �̷��� wait camera ������ ���� ��ٷ� �ִ� ���̴�.
		if (m_wait_resource_render_cameras.empty() == false)
		{
			for (const auto& camera : m_wait_resource_render_cameras)
			{
				if (camera == m_ready_main_camera)
				{
					m_main_camera = m_ready_main_camera;
					m_ready_main_camera = nullptr;
				}
				m_render_cameras.push_back(camera);
			}
			m_wait_resource_render_cameras.clear();
		}

		if (m_ready_render_cameras.empty() == false)
		{
			for (const auto& camera : m_ready_render_cameras)
			{
				IVec2 size = IVec2(camera->GetViewport().width, camera->GetViewport().height);

				if(camera->GetRenderTexture() == nullptr)
					camera->SetRenderTexture(CreateSPtr<RenderTexture>(size));
				else
					camera->GetRenderTexture()->SetTextureSize(size);
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetRenderTexture());
				camera->SetRequireResizeTexture(false);

				m_wait_resource_render_cameras.push_back(camera);
			}
			m_ready_render_cameras.clear();
		}	
	}

	void RenderCameraManager::UpdateRenderCamerasForCascadeShadow(ID3D12Device* device)
	{
		const auto& light_manager = LightManager::GetLightManager();
		const auto& ready_directional_lights = light_manager.GetReadyDirectionalLights();
		if (ready_directional_lights.empty() == false)
		{
			for (const auto& directional_light : ready_directional_lights)
			{
				if (directional_light->IsUseShadow())
				{
					for (const auto& camera : m_ready_render_cameras)
						directional_light->RegisterCascadeAndRenderCamera(camera, CreateSPtr<ShadowCascadeCameraComponent>());
					for (const auto& camera : m_wait_resource_render_cameras)
						directional_light->RegisterCascadeAndRenderCamera(camera, CreateSPtr<ShadowCascadeCameraComponent>());
					for (const auto& camera : m_render_cameras)
						directional_light->RegisterCascadeAndRenderCamera(camera, CreateSPtr<ShadowCascadeCameraComponent>());
				}
			}
		}

		for (const auto& camera : m_wait_resource_render_cameras)
		{
			const auto& directional_lights = light_manager.GetDirectionalLights();
			if (directional_lights.empty() == false)
			{
				for (const auto& directional_light : directional_lights)
				{
					if (directional_light->IsUseShadow())
					{
						directional_light->RegisterCascadeAndRenderCamera(camera, CreateSPtr<ShadowCascadeCameraComponent>());
					}
				}
			}
		}
	}

	void RenderCameraManager::UpdateCameraResource(ID3D12Device* device,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_render_camera)
	{
		const auto& camera_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetCameraFrameResource();
		const auto& camera_resource_data = camera_resource->GetRenderCameraData();

		UINT count = static_cast<UINT>(m_render_cameras.size() + 1);
		if (camera_resource->GetSizeOfRenderCamera() < count)
		{
			camera_resource_data->CreateResource(device, count);
			camera_resource->SetSizeOfRenderCamera(count);
		}

		const auto& light_manager = LightManager::GetLightManager();
		UINT num_of_directional_light = static_cast<UINT>(light_manager.GetDirectionalLights().size());

		RSRenderCameraData camera_data;
		UINT index = 0;

		for (const auto& camera : m_render_cameras)
		{
			if (camera->GetCameraState() == eCameraState::kActive)
			{
				const auto& render_texture = camera->GetRenderTexture();

				if (render_texture->GetResource() != nullptr)
				{
					camera_data.view_matrix = mat4::Transpose(camera->GetViewMatrix());
					camera_data.projection_matrix = mat4::Transpose(camera->GetProjectionMatrix());
					camera_data.view_projection_matrix = mat4::Transpose(camera->GetViewProjectionMatrix());
					camera_data.inverse_view_matrix = mat4::Transpose(camera->GetInverseViewMatrix());
					camera_data.perspective_values = Vec4(1.0f / camera_data.projection_matrix._11,
						1.0f / camera_data.projection_matrix._22,
						camera_data.projection_matrix._34,
						-camera_data.projection_matrix._33);
					camera_data.camera_position = camera->GetCameraPosition();
					camera_data.final_texture_index = render_texture->GetResourceIndex();
					camera_data.gbuffer_texture_indices = XMUINT4(
						render_texture->GetGBufferResourceIndex(0), render_texture->GetGBufferResourceIndex(1),
						render_texture->GetGBufferResourceIndex(2), render_texture->GetDSVResourceIndex());
					camera_data.num_of_directional_light = num_of_directional_light;
					camera_data.render_camera_index = index;
					camera_resource_data->CopyData(index, camera_data);

					if (camera == m_main_camera)
					{
						camera_data.projection_matrix = mat4::Transpose(camera->GetOrthoMatrix());
						camera_resource_data->CopyData(static_cast<UINT>(m_render_cameras.size()), camera_data);
					}

					MeshVisualizer::UpdateVisibilityFromMovableRenderCamera(camera);
					update_shader_function_for_render_camera(device);
				}
			}

			++index;
		}
	}

	void RenderCameraManager::UpdateMainCameraViewport(LONG width, LONG height)
	{
		//Resize (Texture �����)�� �������� �ʴ� �̻� ���ǹ��� �ڵ�
		if (m_ready_main_camera != nullptr)
		{
			m_ready_main_camera->UpdateViewport(0, 0, width, height);
			m_ready_main_camera->SetNearZ(20.0f);
		}
		else if (m_main_camera != nullptr)
		{
			m_main_camera->UpdateViewport(0, 0, width, height);
			m_main_camera->SetNearZ(20.0f);
		}
	}

	void RenderCameraManager::Draw(ID3D12GraphicsCommandList* command_list,
		std::function<void(ID3D12GraphicsCommandList*)>&& before_deferred_function,
		std::function<void(ID3D12GraphicsCommandList*)>&& deferred_function, 
		std::function<void(ID3D12GraphicsCommandList*)>&& after_deferred_function)
	{
		const auto& camera_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetCameraFrameResource();

		UINT index = 0;

		for (const auto& camera : m_render_cameras)
		{
			D3D12_GPU_VIRTUAL_ADDRESS gpu_address;

			if (camera->GetCameraState() == eCameraState::kActive)
			{
				const auto& render_texture = camera->GetRenderTexture();

				if (render_texture->GetResource() != nullptr)
				{
					gpu_address = camera_resource->GetRenderCameraData()->GetResource()->GetGPUVirtualAddress() +
						index * camera_resource->GetRenderCameraData()->GetByteSize();

					//�ϴ� �ӽ�, ���Ŀ� resize�۾��� �ϰ� �Ǹ� ����
					const auto& cv = camera->GetRenderTexture()->GetTextureSize();
					D3D12_VIEWPORT view = { 0.f, 0.f, static_cast<float>(cv.x), static_cast<float>(cv.y), 0.0f, 1.0f };
					D3D12_RECT scissor = { 0, 0, static_cast<LONG>(cv.x), static_cast<LONG>(cv.y) };
					command_list->RSSetViewports(1, &view);
					command_list->RSSetScissorRects(1, &scissor);

					command_list->SetGraphicsRootConstantBufferView(2, gpu_address);

					camera->GetRenderTexture()->GBufferPreDraw(command_list);
					before_deferred_function(command_list);
					camera->GetRenderTexture()->GBufferPostDraw(command_list);

					camera->GetRenderTexture()->PreDraw(command_list);
					deferred_function(command_list);
					camera->GetRenderTexture()->PostDraw(command_list);
				}
			}
			++index;
		}
	}

	void RenderCameraManager::DrawMainCameraForUI(ID3D12GraphicsCommandList* command_list)
	{
		const auto& camera_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetCameraFrameResource();

		command_list->SetGraphicsRootConstantBufferView(2, camera_resource->GetRenderCameraData()->GetResource()->GetGPUVirtualAddress() +
			(m_render_cameras.size()) * camera_resource->GetRenderCameraData()->GetByteSize());
	}

	bool RenderCameraManager::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		m_ready_render_cameras.push_back(std::static_pointer_cast<RenderCameraComponent>(camera_comp));
		return true;
	}

	void RenderCameraManager::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		if (camera_comp == m_main_camera)
			m_main_camera = nullptr;

		UnregisterCameraComponent(m_ready_render_cameras, camera_comp);
		UnregisterCameraComponent(m_wait_resource_render_cameras, camera_comp);
		UnregisterCameraComponent(m_render_cameras, camera_comp);
		UnregisterRenderCameraToDirectionalLights(std::static_pointer_cast<RenderCameraComponent>(camera_comp));
	}

	void RenderCameraManager::UnregisterRenderCameraToDirectionalLights(const SPtr<RenderCameraComponent>& camera_comp)
	{
		const auto& light_manager = LightManager::GetLightManager();
		const auto& ready_directional_lights = light_manager.GetReadyDirectionalLights();

		for (const auto& light : ready_directional_lights)
			light->UnregisterRenderCamera(camera_comp);

		const auto& directional_lights = light_manager.GetDirectionalLights();

		for (const auto& light : directional_lights)
			light->UnregisterRenderCamera(camera_comp);
	}

}
