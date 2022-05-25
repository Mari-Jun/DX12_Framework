#include "stdafx.h"
#include "client/object/component/light/point_light_component.h"
#include "client/object/actor/core/actor.h"
#include "client/object/component/util/shadow_cube_camera_component.h"

namespace client_fw
{
	PointLightComponent::PointLightComponent(const std::string& name,
		const std::string& draw_shader_name)
		: LocalLightComponent(eLightType::kPoint, name, draw_shader_name)
	{
		m_shadow_cube_camera = CreateSPtr<ShadowCubeCameraComponent>();
	}

	bool PointLightComponent::Initialize()
	{
		bool ret = LocalLightComponent::Initialize();

		if(m_is_use_shadow)
			ret &= m_owner.lock()->AttachComponent(m_shadow_cube_camera);

		return ret;
	}

	void PointLightComponent::Shutdown()
	{
		m_shadow_cube_camera = nullptr;
		LocalLightComponent::Shutdown();
	}

	void PointLightComponent::UpdateWorldMatrix()
	{
		const auto& owner = m_owner.lock();
		if (owner != nullptr)
		{
			const auto& world = owner->GetWorldMatrix();
			m_world_position = vec3::TransformCoord(m_local_position, world);
			m_world_rotation = m_local_rotation * owner->GetRotation();
			m_world_scale = m_local_scale * owner->GetScale();

			m_world_matrix = mat4::CreateScale(m_attenuation_radius * 1.1f);
			m_world_matrix *= mat4::CreateTranslation(m_world_position);

			UpdateOrientedBox();

			m_is_updated_world_matrix = true;
		}
	}

	void PointLightComponent::UpdateLocalMatrix()
	{
		if (m_update_local_matrix)
		{
			m_local_matrix = mat4::CreateTranslation(m_local_position);
			m_update_local_matrix = false;

			m_shadow_cube_camera->SetLocalPosition(m_local_position);

			UpdateWorldMatrix();
		}
	}

	void PointLightComponent::UpdateOrientedBox()
	{
		m_oriented_box->Transform(BOrientedBox(), GetWorldMatrix());
	}

	void PointLightComponent::SetVisiblity(bool value)
	{
		m_visibility = value;
		if (value)
			m_shadow_cube_camera->SetActive();
	}

	void PointLightComponent::UpdateShadowTextureSize()
	{
		INT extent = std::clamp(m_shadow_texture_size, 0, s_point_light_max_texture_size);
		m_shadow_cube_camera->SetViewSize(IVec2(extent, extent));
	}

	void PointLightComponent::UpdateShadowCameraProjection()
	{
		m_shadow_cube_camera->SetFarZ(m_attenuation_radius);
	}
}
