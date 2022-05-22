#include "stdafx.h"
#include "client/renderer/core/light_manager.h"

#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/light_frame_resource.h"
#include "client/renderer/frameresource/shadow_frame_resource.h"

#include "client/object/component/light/core/light_component.h"
#include "client/object/component/light/directional_light_component.h"
#include "client/object/component/light/point_light_component.h"
#include "client/object/component/light/spot_light_component.h"
#include "client/object/component/util/shadow_camera_component.h"
#include "client/object/component/util/shadow_cube_camera_component.h"
#include "client/object/component/util/shadow_cascade_camera_component.h"
#include "client/object/component/util/render_camera_component.h"

#include "client/object/actor/core/actor.h"

#include "client/asset/texture/texture.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	LightManager* LightManager::s_light_manager = nullptr;

	std::array<Mat4, 1> LightManager::s_uv_from_ndc_matrix = { 
		Mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f)
	};

	LightManager::LightManager()
	{
		s_light_manager = this;
	}

	LightManager::~LightManager()
	{
	}

	void LightManager::Shutdown()
	{
		s_light_manager = nullptr;
	}

	void LightManager::Update(ID3D12Device* device)
	{
		const auto& light_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetLightFrameResource();
		const auto& light_resource_data = light_resource->GetLightData();

		UINT size_of_light = light_resource->GetSizeOfLight();

		bool is_need_create_resource = false;
		while (size_of_light < m_num_of_light)
		{
			size_of_light = static_cast<UINT>(roundf(static_cast<float>(size_of_light) * 1.5f));
			is_need_create_resource = true;
		}

		if (is_need_create_resource)
		{
			light_resource_data->CreateResource(device, size_of_light);
			light_resource->SetSizeOfLight(size_of_light);
			is_need_create_resource = false;
		}

		const auto& shadow_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetShadowFrameResource();
		UINT num_of_cascade_shadow_texture = 0;

		for (const auto& light : m_directional_lights)
			num_of_cascade_shadow_texture += static_cast<UINT>(light->GetCascadeShadowsCameras().size());

		m_num_of_cascade_shadow_texture = max(m_num_of_cascade_shadow_texture, num_of_cascade_shadow_texture);

		const auto& cascade_shadow_texture_resource_data = shadow_resource->GetCascadeShadowTextureData();
		UINT size_of_cascade_shadow_texture = shadow_resource->GetSizeOfCascadeShadowTexture();

		while (size_of_cascade_shadow_texture < m_num_of_cascade_shadow_texture)
		{
			size_of_cascade_shadow_texture = static_cast<UINT>(roundf(static_cast<float>(size_of_cascade_shadow_texture) * 1.5f));
			is_need_create_resource = true;
		}

		if (is_need_create_resource)
		{
			cascade_shadow_texture_resource_data->CreateResource(device, size_of_cascade_shadow_texture);
			shadow_resource->SetSizeOfCascadeShadowTexture(size_of_cascade_shadow_texture);
			is_need_create_resource = false;
		}

		const auto& shadow_texture_resource_data = shadow_resource->GetShadowTextureData();
		UINT size_of_shadow_texture = shadow_resource->GetSizeOfShadowTexture();

		while (size_of_shadow_texture < m_num_of_shadow_texture + m_num_of_cascade_shadow_texture)
		{
			size_of_shadow_texture = static_cast<UINT>(roundf(static_cast<float>(size_of_shadow_texture) * 1.5f));
			is_need_create_resource = true;
		}

		if (is_need_create_resource)
		{
			shadow_texture_resource_data->CreateResource(device, size_of_shadow_texture);
			shadow_resource->SetSizeOfShadowTexture(size_of_shadow_texture);
			is_need_create_resource = false;
		}

		std::vector<RSLightData> lights_data;
		std::vector<RSShadowTextureData> shadow_textures_data;
		std::vector<RSCascadeShadowTextureData> cascade_shadow_textures_data;

		UINT light_index = 0;
		UINT shadow_index = 0;
		
		std::move(m_ready_directional_lights.begin(), m_ready_directional_lights.end(), std::back_inserter(m_directional_lights));
		m_ready_directional_lights.clear();

		for (const auto& light : m_directional_lights)
		{
			RSLightData light_data;
			light_data.light_color = light->GetLightColor() * light->GetLightIntensity();
			light_data.light_direction = light->GetOwner().lock()->GetForward();
			light->SetLightManagerRegisteredIndex(light_index++);
			light_data.use_shadow = light->IsUseShadow() & light->GetShadowVisibility();
			light_data.shadow_texture_data_index = shadow_index;
			lights_data.emplace_back(std::move(light_data));

			const auto& cascade_shadow_cameras = light->GetCascadeShadowsCameras();
			for (const auto& cascade_shadow_camera_info : cascade_shadow_cameras)
			{
				const auto& render_camera = cascade_shadow_camera_info.render_camera;
				const auto& cascade_camera = cascade_shadow_camera_info.shadow_cascade_camera;
				if (light_data.use_shadow == false)
					cascade_camera->SetPaused();
				else
					cascade_camera->SetActive();

				const auto& shadow_texture = cascade_camera->GetShadowArrayTexture();

				if (shadow_texture != nullptr)
				{
					RSShadowTextureData shadow_texture_data;
					shadow_texture_data.shadow_texture_index = shadow_texture->GetResourceIndex();
					shadow_texture_data.uv_from_ndc_matrix = mat4::Transpose(cascade_camera->GetViewProjectionMatrix());

					shadow_textures_data.emplace_back(std::move(shadow_texture_data));

					RSCascadeShadowTextureData cascade_shadow_texture_data;
					for (UINT i = 0; i < s_max_cascade_level; ++i)
					{
						cascade_shadow_texture_data.cascade_offset_x[i] = cascade_camera->GetCascadeXOffset()[i];
						cascade_shadow_texture_data.cascade_offset_y[i] = cascade_camera->GetCascadeYOffset()[i];
						cascade_shadow_texture_data.cascade_scale[i] = cascade_camera->GetCascadeScale()[i];
					}
					for (UINT i = s_max_cascade_level; i < 4; ++i)
					{
						cascade_shadow_texture_data.cascade_offset_x[i] = FLT_MAX;
						cascade_shadow_texture_data.cascade_offset_y[i] = FLT_MAX;
						cascade_shadow_texture_data.cascade_scale[i] = 0.1f;
					}
					cascade_shadow_textures_data.emplace_back(std::move(cascade_shadow_texture_data));
					++shadow_index;
				}
			}
		}

		for (const auto& light : m_point_lights)
		{
			RSLightData light_data;
			light_data.light_color = light->GetLightColor() * light->GetLightIntensity();
			light_data.light_position = light->GetWorldPosition();
			light_data.attenuation_radius = light->GetAttenuationRadius();
			light_data.use_shadow = light->IsUseShadow() & light->GetShadowVisibility();
			light_data.is_static_light = (light->GetOwner().lock()->GetMobilityState() != eMobilityState::kMovable);
			light_data.shadow_texture_data_index = shadow_index;
			light->SetLightManagerRegisteredIndex(light_index++);
			lights_data.emplace_back(std::move(light_data));

			const auto& shadow_cube_camera = light->GetShadowCubeCamera();
			if (light_data.use_shadow == false)
				shadow_cube_camera->SetPaused();
			const auto& shadow_texture = shadow_cube_camera->GetShadowCubeTexture();
			if (shadow_texture != nullptr)
			{
				const auto& projection = shadow_cube_camera->GetProjectionMatrix();

				RSShadowTextureData shadow_texture_data;
				shadow_texture_data.shadow_texture_index = shadow_texture->GetResourceIndex();
				shadow_texture_data.inverse_texture_size = Vec2(projection._33, projection._43);

				shadow_textures_data.emplace_back(std::move(shadow_texture_data));
				++shadow_index;
			}
		}

		for (const auto& light : m_spot_lights)
		{
			RSLightData light_data;
			light_data.light_color = light->GetLightColor() * light->GetLightIntensity();
			light_data.light_position = light->GetWorldPosition();
			light_data.light_direction = light->GetWorldForward();
			light_data.attenuation_radius = light->GetAttenuationRadius();
			light_data.cone_inner_angle = light->GetConeInnerAngle();
			light_data.cone_outer_angle = light->GetConeOuterAngle();
			light_data.use_shadow = light->IsUseShadow() & light->GetShadowVisibility();
			light_data.is_static_light = (light->GetOwner().lock()->GetMobilityState() != eMobilityState::kMovable);
			light_data.shadow_texture_data_index = shadow_index;
			light->SetLightManagerRegisteredIndex(light_index++);
			lights_data.emplace_back(std::move(light_data));

			const auto& shadow_camera = light->GetShadowCamera();
			if (light_data.use_shadow == false)
				shadow_camera->SetPaused();
			const auto& shadow_texture = shadow_camera->GetShadowTexture();
			if (shadow_texture != nullptr)
			{
				RSShadowTextureData shadow_texture_data;
				shadow_texture_data.shadow_texture_index = shadow_texture->GetResourceIndex();
				shadow_texture_data.inverse_texture_size = Vec2(
					1.0f / static_cast<float>(shadow_texture->GetTextureSize().x),
					1.0f / static_cast<float>(shadow_texture->GetTextureSize().y));
				shadow_texture_data.uv_from_ndc_matrix = mat4::Transpose(
					shadow_camera->GetViewProjectionMatrix() * s_uv_from_ndc_matrix[0]
				);

				shadow_textures_data.emplace_back(std::move(shadow_texture_data));
				++shadow_index;
			}
		}

		light_resource_data->CopyVectorData(std::move(lights_data));
		shadow_texture_resource_data->CopyVectorData(std::move(shadow_textures_data));
		cascade_shadow_texture_resource_data->CopyVectorData(std::move(cascade_shadow_textures_data));
	}

	void LightManager::Draw(ID3D12GraphicsCommandList* command_list)
	{
		if (m_num_of_light > 0)
		{
			const auto& light_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetLightFrameResource();
			const auto& light_resource_data = light_resource->GetLightData();

			command_list->SetGraphicsRootShaderResourceView(5, light_resource_data->GetResource()->GetGPUVirtualAddress());

			const auto& shadow_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetShadowFrameResource();
			const auto& shadow_texture_resource_data = shadow_resource->GetShadowTextureData();

			command_list->SetGraphicsRootShaderResourceView(6, shadow_texture_resource_data->GetResource()->GetGPUVirtualAddress());

			const auto& cascade_shadow_texture_resource_data = shadow_resource->GetCascadeShadowTextureData();

			command_list->SetGraphicsRootShaderResourceView(9, cascade_shadow_texture_resource_data->GetResource()->GetGPUVirtualAddress());
		}
	}

	bool LightManager::RegisterLightComponent(const SPtr<LightComponent>& light_comp)
	{
		switch (light_comp->GetLightType())
		{
		case eLightType::kDirectional:
		{
			if (m_directional_lights.size() + m_ready_directional_lights.size() < s_max_directional_light)
			{
				RegisterLightComponent(m_ready_directional_lights, light_comp);
				if (light_comp->IsUseShadow())
					--m_num_of_shadow_texture;
				//CameraManager::GetCameraManager();
			}
			else
				return false;
			break;
		}
		case eLightType::kPoint:
		{
			RegisterLightComponent(m_point_lights, light_comp);
			break;
		}
		case eLightType::kSpot:
		{
			RegisterLightComponent(m_spot_lights, light_comp);
			break;
		}
		default:
			break;
		}
		return true;
	}

	void LightManager::UnregisterLightComponent(const SPtr<LightComponent>& light_comp)
	{
		switch (light_comp->GetLightType())
		{
		case eLightType::kDirectional:
		{
			UnregisterLightComponent(m_directional_lights, light_comp);
			++m_num_of_shadow_texture;
			break;
		}
		case eLightType::kPoint:
		{
			UnregisterLightComponent(m_point_lights, light_comp);
			break;
		}
		case eLightType::kSpot:
		{
			UnregisterLightComponent(m_spot_lights, light_comp);
			break;
		}
		default:
			break;
		}
	}
}
