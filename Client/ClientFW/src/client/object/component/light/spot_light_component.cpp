#include "stdafx.h"
#include "client/object/component/light/spot_light_component.h"
#include "client/object/actor/core/actor.h"
#include "client/object/component/util/shadow_camera_component.h"

namespace client_fw
{
	SpotLightComponent::SpotLightComponent(const std::string& name,
		const std::string& draw_shader_name)
		: LocalLightComponent(eLightType::kSpot, name, draw_shader_name)
	{
		m_shadow_camera = CreateSPtr<ShadowCameraComponent>();
	}

	bool SpotLightComponent::Initialize()
	{
		bool ret = LocalLightComponent::Initialize();

		if (m_is_use_shadow)
			ret &= m_owner.lock()->AttachComponent(m_shadow_camera);

		return ret;
	}

	void SpotLightComponent::Shutdown()
	{
		m_shadow_camera = nullptr;
		LocalLightComponent::Shutdown();
	}

	void SpotLightComponent::UpdateWorldMatrix()
	{
		const auto& owner = m_owner.lock();
		if (owner != nullptr)
		{
			const auto& world = owner->GetWorldMatrix();
			m_world_position = vec3::TransformCoord(m_local_position, world);
			m_world_rotation = m_local_rotation * owner->GetRotation();
			m_world_scale = m_local_scale * owner->GetScale();

			m_world_matrix = mat4::CreateScale(m_attenuation_radius * 1.1f);
			m_world_matrix *= mat4::CreateRotationFromQuaternion(m_world_rotation);
			m_world_matrix *= mat4::CreateTranslation(m_world_position);

			UpdateOrientedBox();

			m_is_updated_world_matrix = true;
		}
	}

	void SpotLightComponent::UpdateLocalMatrix()
	{
		if (m_update_local_matrix)
		{
			m_local_matrix = mat4::CreateRotationFromQuaternion(m_local_rotation);
			m_local_matrix *= mat4::CreateTranslation(m_local_position);
			m_update_local_matrix = false;

			m_shadow_camera->SetLocalPosition(m_local_position);
			m_shadow_camera->SetLocalRotation(m_local_rotation);

			UpdateWorldMatrix();
		}
	}

	void SpotLightComponent::UpdateOrientedBox()
	{
		m_oriented_box->Transform(
			BOrientedBox(Vec3(0.0f, 0.0f, 0.5f), Vec3(sinf(m_cone_outer_angle), sinf(m_cone_outer_angle), 0.5f)),
			GetWorldMatrix());
	}

	void SpotLightComponent::SetVisiblity(bool value)
	{
		m_visibility = value;
		if (value)
			m_shadow_camera->SetActive();
	}

	void SpotLightComponent::SetConeInnerAngle(float degrees)
	{
		m_cone_inner_angle = math::ToRadian(degrees);
		m_cone_outer_angle = max(m_cone_inner_angle, m_cone_outer_angle);
		m_shadow_camera->SetFieldOfView(math::ToDegrees(m_cone_outer_angle) * 2.0f);
		m_update_local_matrix = true;
	}

	void SpotLightComponent::SetConeOuterAngle(float degrees)
	{
		m_cone_outer_angle = math::ToRadian(degrees);
		m_cone_inner_angle = min(m_cone_inner_angle, m_cone_outer_angle);
		m_shadow_camera->SetFieldOfView(degrees * 2.0f);
		m_update_local_matrix = true;
	}

	void SpotLightComponent::UpdateShadowTextureSize()
	{
		INT extent = std::clamp(m_shadow_texture_size, 0, s_spot_light_max_texture_size);
		m_shadow_camera->SetViewSize(IVec2(extent, extent));
	}

	void SpotLightComponent::UpdateShadowCameraProjection()
	{
		m_shadow_camera->SetFarZ(m_attenuation_radius);
	}
}
