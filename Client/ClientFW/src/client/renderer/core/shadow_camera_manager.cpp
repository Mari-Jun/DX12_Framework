#include "stdafx.h"
#include "client/renderer/core/render_camera_manager.h"
#include "client/renderer/core/light_manager.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/camera_frame_resource.h"
#include "client/renderer/core/mesh_visualizer.h"

#include "client/object/actor/core/actor.h"
#include "client/object/component/util/render_camera_component.h"
#include "client/object/component/util/shadow_camera_component.h"
#include "client/object/component/util/shadow_cube_camera_component.h"
#include "client/object/component/util/shadow_cascade_camera_component.h"
#include "client/object/component/light/directional_light_component.h"

#include "client/asset/texture/texture.h"
#include "client/util/upload_buffer.h"
#include "shadow_camera_manager.h"

namespace client_fw
{
	ShadowCameraManager::ShadowCameraManager()
	{
	}

	ShadowCameraManager::~ShadowCameraManager()
	{
	}

	void ShadowCameraManager::Update(ID3D12Device* device,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_camera,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_cube_camera,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_cascade_camera)
	{
		UpdateShadowCameras(device);
		UpdateShadowCubeCameras(device);
		UpdateShadowCascadeCameras(device);

		UpdateCameraResource(device,
			std::move(update_shader_function_for_shadow_camera),
			std::move(update_shader_function_for_shadow_cube_camera),
			std::move(update_shader_function_for_shadow_cascade_camera));
	}


	void ShadowCameraManager::UpdateShadowCameras(ID3D12Device* device)
	{
		if (m_wait_resource_shadow_cameras.empty() == false)
		{
			for (const auto& camera : m_wait_resource_shadow_cameras)
			{
				m_shadow_cameras.push_back(camera);
				if (camera->GetOwner().lock()->GetMobilityState() != eMobilityState::kMovable)
					m_need_write_static_depth_shadow_cameras.push_back(camera);
			}
			m_wait_resource_shadow_cameras.clear();
		}

		if (m_ready_shadow_cameras.empty() == false)
		{
			for (const auto& camera : m_ready_shadow_cameras)
			{
				IVec2 size = IVec2(camera->GetViewport().width, camera->GetViewport().height);

				camera->SetShadowTexture(CreateSPtr<Shadow2DTexture>(size));
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetShadowTexture());
				if (camera->GetOwner().lock()->GetMobilityState() != eMobilityState::kMovable)
				{
					camera->SetStaticShadowTexture(CreateSPtr<Shadow2DTexture>(size));
					RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetStaticShadowTexture());
				}

				m_wait_resource_shadow_cameras.push_back(camera);
			}
			m_ready_shadow_cameras.clear();
		}
	}

	void ShadowCameraManager::UpdateShadowCubeCameras(ID3D12Device* device)
	{
		if (m_wait_resource_shadow_cube_cameras.empty() == false)
		{
			for (const auto& camera : m_wait_resource_shadow_cube_cameras)
			{
				m_shadow_cube_cameras.push_back(camera);
				if (camera->GetOwner().lock()->GetMobilityState() != eMobilityState::kMovable)
					m_need_write_static_depth_shadow_cube_cameras.push_back(camera);
			}
			m_wait_resource_shadow_cube_cameras.clear();
		}

		if (m_ready_shadow_cube_cameras.empty() == false)
		{
			for (const auto& camera : m_ready_shadow_cube_cameras)
			{
				IVec2 size = IVec2(camera->GetViewport().width, camera->GetViewport().height);

				camera->SetShadowCubeTexture(CreateSPtr<ShadowCubeTexture>(size));
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetShadowCubeTexture());
				if (camera->GetOwner().lock()->GetMobilityState() != eMobilityState::kMovable)
				{
					camera->SetStaticShadowCubeTexture(CreateSPtr<ShadowCubeTexture>(size));
					RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetStaticShadowCubeTexture());
				}

				m_wait_resource_shadow_cube_cameras.push_back(camera);
			}
			m_ready_shadow_cube_cameras.clear();
		}
	}

	void ShadowCameraManager::UpdateShadowCascadeCameras(ID3D12Device* device)
	{
		if (m_wait_resource_shadow_cascade_cameras.empty() == false)
		{
			for (const auto& camera : m_wait_resource_shadow_cascade_cameras)
				m_shadow_cascade_cameras.push_back(camera);
			m_wait_resource_shadow_cascade_cameras.clear();
		}

		if (m_ready_shadow_cascade_cameras.empty() == false)
		{
			for (const auto& camera : m_ready_shadow_cascade_cameras)
			{
				IVec2 size = IVec2(camera->GetViewport().width, camera->GetViewport().height);

				camera->SetShadowArrayTexture(CreateSPtr<ShadowArrayTexture>(size, s_max_cascade_level));
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(camera->GetShadowArrayTexture());

				m_wait_resource_shadow_cascade_cameras.push_back(camera);
			}
			m_ready_shadow_cascade_cameras.clear();
		}
	}

	void ShadowCameraManager::UpdateCameraResource(ID3D12Device* device, 
		std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_camera,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_cube_camera,
		std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_cascade_camera)
	{
		const auto& camera_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetCameraFrameResource();
		const auto& camera_resource_data = camera_resource->GetShadowCameraData();

		UINT count = static_cast<UINT>(m_shadow_cameras.size() +
			m_shadow_cube_cameras.size() * 6 + 
			m_shadow_cascade_cameras.size() * s_max_cascade_level);

		count += static_cast<UINT>(m_need_write_static_depth_shadow_cameras.size() +
			m_need_write_static_depth_shadow_cube_cameras.size() * 6);

		if (camera_resource->GetSizeOfShadowCamera() < count)
		{
			camera_resource_data->CreateResource(device, count);
			camera_resource->SetSizeOfShadowCamera(count);
		}

		std::vector<RSShadowCameraData> cameras_data;
		RSShadowCameraData camera_data;

		for (const auto& camera : m_need_write_static_depth_shadow_cameras)
		{
			const auto& shadow_texture = camera->GetStaticShadowTexture();

			if (shadow_texture->GetResource() != nullptr)
			{
				cameras_data.emplace_back(RSShadowCameraData{ mat4::Transpose(camera->GetViewProjectionMatrix()) });
				MeshVisualizer::UpdateVisibilityFromStaticShadowCamera(camera, true);

				update_shader_function_for_shadow_camera(device);
			}
		}

		for (const auto& camera : m_shadow_cameras)
		{
			const auto& shadow_texture = camera->GetShadowTexture();

			if (camera->GetCameraState() == eCameraState::kActive &&
				shadow_texture->GetResource() != nullptr)
			{
				cameras_data.emplace_back(RSShadowCameraData{ mat4::Transpose(camera->GetViewProjectionMatrix()) });

				if (camera->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
					MeshVisualizer::UpdateVisibilityFromMovableShadowCamera(camera);
				else
					MeshVisualizer::UpdateVisibilityFromStaticShadowCamera(camera);

				update_shader_function_for_shadow_camera(device);
			}
		}

		for (const auto& camera : m_need_write_static_depth_shadow_cube_cameras)
		{
			const auto& shadow_cube_texture = camera->GetStaticShadowCubeTexture();

			if (shadow_cube_texture->GetResource() != nullptr)
			{
				for (UINT i = 0; i < 6; ++i)
				{
					cameras_data.emplace_back(RSShadowCameraData{
						mat4::Transpose(camera->GetCubeViewMatrixs()[i] * camera->GetProjectionMatrix()) });
				}

				MeshVisualizer::UpdateVisibilityFromStaticShadowSphere(camera, camera->GetWorldPosition(), camera->GetFarZ(), true);
				update_shader_function_for_shadow_cube_camera(device);
			}
		}

		for (const auto& camera : m_shadow_cube_cameras)
		{
			const auto& shadow_cube_texture = camera->GetShadowCubeTexture();

			if (camera->GetCameraState() == eCameraState::kActive &&
				shadow_cube_texture->GetResource() != nullptr)
			{
				for (UINT i = 0; i < 6; ++i)
				{
					cameras_data.emplace_back(RSShadowCameraData{
						mat4::Transpose(camera->GetCubeViewMatrixs()[i] * camera->GetProjectionMatrix()) });
				}

				if (camera->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
					MeshVisualizer::UpdateVisibilityFromMovableShadowSphere(camera, camera->GetWorldPosition(), camera->GetFarZ());
				else
					MeshVisualizer::UpdateVisibilityFromStaticShadowSphere(camera, camera->GetWorldPosition(), camera->GetFarZ());

				update_shader_function_for_shadow_cube_camera(device);
			}
		}

		for (const auto& camera : m_shadow_cascade_cameras)
		{
			if (camera->GetCameraState() == eCameraState::kActive && 
				camera->GetRenderCamera().lock()->GetCameraState() == eCameraState::kActive)
			{
				const auto& shadow_cascade_texture = camera->GetShadowArrayTexture();

				if (shadow_cascade_texture->GetResource() != nullptr)
				{
					for (UINT i = 0; i < 3; ++i)
					{
						cameras_data.emplace_back(RSShadowCameraData{
							mat4::Transpose(camera->GetWorldToCascadeMatrix()[i])
							});
					}

					// Cascade Shadow뿐만 아니라 다른 Shadow도 최적화가 필요하지만 일단 기능 구현에 목적을 두겠다.
					MeshVisualizer::UpdateVisibilityFromMovableShadowSphere(camera,
						camera->GetCascadeBoundingSphere().GetCenter(), camera->GetCascadeBoundingSphere().GetRadius());
					update_shader_function_for_shadow_cascade_camera(device);
				}
			}
		}

		camera_resource_data->CopyVectorData(std::move(cameras_data));
	}

	void ShadowCameraManager::Draw(ID3D12GraphicsCommandList* command_list,
		std::function<void(ID3D12GraphicsCommandList*)>&& shadow_function, 
		std::function<void(ID3D12GraphicsCommandList*)>&& shadow_cube_function,
		std::function<void(ID3D12GraphicsCommandList*)>&& shadow_cascade_function)
	{
		const auto& camera_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetCameraFrameResource();

		D3D12_GPU_VIRTUAL_ADDRESS gpu_address = camera_resource->GetShadowCameraData()->GetResource()->GetGPUVirtualAddress();
		UINT gpu_offset = camera_resource->GetShadowCameraData()->GetByteSize();

		constexpr static auto trigger_function =
			[](const SPtr<RenderComponent>& render_cmp)
		{
			return (render_cmp->IsVisible() == false && render_cmp->IsHiddenInGame() == false);
		};

		const static auto DrawShadowTexture =
			[command_list, shadow_function, &gpu_address, gpu_offset](const SPtr<Shadow2DTexture>& shadow_texture)
		{
			if (shadow_texture->GetResource() != nullptr)
			{
				const auto& cv = shadow_texture->GetTextureSize();
				D3D12_VIEWPORT view = { 0.f, 0.f, static_cast<float>(cv.x), static_cast<float>(cv.y), 0.0f, 1.0f };
				D3D12_RECT scissor = { 0, 0, static_cast<LONG>(cv.x), static_cast<LONG>(cv.y) };
				command_list->RSSetViewports(1, &view);
				command_list->RSSetScissorRects(1, &scissor);

				command_list->SetGraphicsRootShaderResourceView(8, gpu_address);

				shadow_texture->PreDraw(command_list);
				shadow_function(command_list);
				shadow_texture->PostDraw(command_list);

				gpu_address += gpu_offset;
			}
		};

		for (const auto& camera : m_need_write_static_depth_shadow_cameras)
		{
			const auto& shadow_texture = camera->GetStaticShadowTexture();
			DrawShadowTexture(shadow_texture);
		}

		for (const auto& camera : m_shadow_cameras)
		{
			if (camera->GetCameraState() == eCameraState::kActive)
			{
				const auto& shadow_texture = camera->GetShadowTexture();
				DrawShadowTexture(shadow_texture);
				camera->SetPaused();
			}
		}

		const static auto DrawShadowCubeTexture =
			[command_list, shadow_cube_function, &gpu_address, gpu_offset](const SPtr<ShadowCubeTexture>& shadow_cube_texture)
		{
			if (shadow_cube_texture->GetResource() != nullptr)
			{
				const auto& size = shadow_cube_texture->GetTextureSize();

				std::array<D3D12_VIEWPORT, 6> views;
				views.fill({ 0.f, 0.f, static_cast<float>(size.x), static_cast<float>(size.y), 0.0f, 1.0f });
				std::array<D3D12_RECT, 6> scissors;
				scissors.fill({ 0, 0, static_cast<LONG>(size.x), static_cast<LONG>(size.y) });

				command_list->RSSetViewports(6, views.data());
				command_list->RSSetScissorRects(6, scissors.data());

				command_list->SetGraphicsRootShaderResourceView(8, gpu_address);

				shadow_cube_texture->PreDraw(command_list);
				shadow_cube_function(command_list);
				shadow_cube_texture->PostDraw(command_list);

				gpu_address += gpu_offset * 6;
			}
		};

		for (const auto& camera : m_need_write_static_depth_shadow_cube_cameras)
		{
			const auto& shadow_cube_texture = camera->GetStaticShadowCubeTexture();
			DrawShadowCubeTexture(shadow_cube_texture);
		}

		for (const auto& camera : m_shadow_cube_cameras)
		{
			if (camera->GetCameraState() == eCameraState::kActive)
			{
				const auto& shadow_cube_texture = camera->GetShadowCubeTexture();
				DrawShadowCubeTexture(shadow_cube_texture);
				camera->SetPaused();
			}
		}

		for (const auto& camera : m_shadow_cascade_cameras)
		{
			if (camera->GetRenderCamera().lock()->GetCameraState() == eCameraState::kActive)
			{
				const auto& shadow_cascade_texture = camera->GetShadowArrayTexture();

				if (shadow_cascade_texture->GetResource() != nullptr)
				{
					const auto& size = shadow_cascade_texture->GetTextureSize();

					std::array<D3D12_VIEWPORT, s_max_cascade_level> views;
					views.fill({ 0.f, 0.f, static_cast<float>(size.x), static_cast<float>(size.y), 0.0f, 1.0f });
					std::array<D3D12_RECT, s_max_cascade_level> scissors;
					scissors.fill({ 0, 0, static_cast<LONG>(size.x), static_cast<LONG>(size.y) });

					command_list->RSSetViewports(s_max_cascade_level, views.data());
					command_list->RSSetScissorRects(s_max_cascade_level, scissors.data());

					command_list->SetGraphicsRootShaderResourceView(8, gpu_address);

					shadow_cascade_texture->PreDraw(command_list);
					shadow_cascade_function(command_list);
					shadow_cascade_texture->PostDraw(command_list);

					gpu_address += gpu_offset * s_max_cascade_level;
				}
			}
		}

		m_need_write_static_depth_shadow_cameras.clear();
		m_need_write_static_depth_shadow_cube_cameras.clear();
	}

	bool ShadowCameraManager::RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		if (camera_comp->GetCameraUsage() == eCameraUsage::kShadow)
			m_ready_shadow_cameras.push_back(std::static_pointer_cast<ShadowCameraComponent>(camera_comp));
		else if (camera_comp->GetCameraUsage() == eCameraUsage::kShadowCube)
			m_ready_shadow_cube_cameras.push_back(std::static_pointer_cast<ShadowCubeCameraComponent>(camera_comp));
		else
			m_ready_shadow_cascade_cameras.push_back(std::static_pointer_cast<ShadowCascadeCameraComponent>(camera_comp));
		return true;
	}

	void ShadowCameraManager::UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp)
	{
		switch (camera_comp->GetCameraUsage())
		{
		case eCameraUsage::kShadow:
		{
			UnregisterCameraComponent(m_ready_shadow_cameras, camera_comp);
			UnregisterCameraComponent(m_wait_resource_shadow_cameras, camera_comp);
			UnregisterCameraComponent(m_shadow_cameras, camera_comp);
			break;
		}
		case eCameraUsage::kShadowCube:
		{
			UnregisterCameraComponent(m_ready_shadow_cube_cameras, camera_comp);
			UnregisterCameraComponent(m_wait_resource_shadow_cube_cameras, camera_comp);
			UnregisterCameraComponent(m_shadow_cube_cameras, camera_comp);
			break;
		}
		case eCameraUsage::kShadowCascade:
		{
			UnregisterCameraComponent(m_ready_shadow_cascade_cameras, camera_comp);
			UnregisterCameraComponent(m_wait_resource_shadow_cascade_cameras, camera_comp);
			UnregisterCameraComponent(m_shadow_cascade_cameras, camera_comp);
			break;
		}
		default:
			break;
		}
	}
}