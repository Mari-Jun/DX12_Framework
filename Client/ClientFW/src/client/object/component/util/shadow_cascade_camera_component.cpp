#include "stdafx.h"
#include "client/object/component/util/shadow_cascade_camera_component.h"
#include "client/object/component/util/render_camera_component.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	ShadowCascadeCameraComponent::ShadowCascadeCameraComponent(const std::string& name)
		: CameraComponent(eCameraUsage::kShadowCascade, name)
	{
		m_camera_state = eCameraState::kActive;
	}

	bool ShadowCascadeCameraComponent::Initialize()
	{
		bool ret = CameraComponent::Initialize();
		return true;
	}

	void ShadowCascadeCameraComponent::Update(float delta_time)
	{
		const auto& render_camera_owner = m_render_camera_component.lock()->GetOwner().lock();

		if (render_camera_owner->IsUpdatedWorldMatrix())
		{
			UpdateViewMatrix();
		}
	}

	void ShadowCascadeCameraComponent::UpdateViewMatrix()
	{
		// onwer is directional light
		const auto& owner = m_owner.lock();
		const auto& render_camera = m_render_camera_component.lock();
		if (render_camera != nullptr)
		{
			m_cascade_far_z = min(render_camera->GetFarZ(), m_max_cascade_far_z);

			Vec3 target, up;

			m_camera_position = render_camera->GetCameraPosition() +
				render_camera->GetCameraForward() * m_cascade_far_z * 0.5f * m_cascade_level_ratio[s_max_cascade_level - 1];
			m_camera_forward = owner->GetForward();
			target = m_camera_position + m_camera_forward;
			up = owner->GetUp();

			m_view_matrix = mat4::LookAt(m_camera_position, target, up);

			m_inverse_view_matrix = mat4::Inverse(m_view_matrix);

			UpdateProjectionMatrix();
		}
		else
		{
			LOG_WARN("render camera is nullptr");
		}
	}

	void ShadowCascadeCameraComponent::UpdateProjectionMatrix()
	{
		const auto& render_camera = m_render_camera_component.lock();
		if (render_camera->GetProjectionMode() == eProjectionMode::kPerspective)
		{
			float field_of_view = math::ToRadian(render_camera->GetFieldOfView());
			float aspect_ratio = render_camera->GetAspectRatio();
			float near_z = render_camera->GetNearZ();
			float far_z = m_cascade_far_z * m_cascade_level_ratio[s_max_cascade_level - 1];


			Mat4 rc_cascade_projection = mat4::Perspective(field_of_view, aspect_ratio, near_z, far_z);
			BFrustum bf_projection = BFrustum(rc_cascade_projection);
			bf_projection.Transform(render_camera->GetInverseViewMatrix());

			// render camera cascade boudning sphere volume
			Vec3 sphere_center = render_camera->GetCameraPosition() + render_camera->GetCameraForward() * (near_z + 0.5f * (near_z + far_z));

			m_cascade_bounding_sphere = BSphere(bf_projection);
			m_cascade_bounding_sphere.SetCenter(sphere_center);
			float radius = m_cascade_bounding_sphere.GetRadius();

			// get projectoin matrix from bounding sphere
			m_projection_matrix = mat4::Ortho(radius, radius, -radius, radius);

			m_bf_projection = BFrustum(m_projection_matrix);
			m_bounding_frustum.Transform(m_bf_projection, render_camera->GetInverseViewMatrix());
			
			//WorldToShadowSpace matrix for cascade
			m_view_projection_matrix = m_view_matrix * m_projection_matrix;

			UpdateCascadeMatrix();
		}
		else
		{
			LOG_WARN("render camera projection mode is not perspective");
		}
	}

	void ShadowCascadeCameraComponent::UpdateCascadeMatrix()
	{
		const auto& render_camera = m_render_camera_component.lock();

		for (UINT i = 0; i < s_max_cascade_level; ++i)
		{
			Mat4 cascade_projection;

			if (i == 0)
			{
				 cascade_projection = mat4::Perspective(math::ToRadian(render_camera->GetFieldOfView()),
					render_camera->GetAspectRatio(), render_camera->GetNearZ(), m_cascade_far_z * m_cascade_level_ratio[i]);
			}
			else
			{
				cascade_projection = mat4::Perspective(math::ToRadian(render_camera->GetFieldOfView()),
					render_camera->GetAspectRatio(), m_cascade_far_z * m_cascade_level_ratio[i - 1], m_cascade_far_z * m_cascade_level_ratio[i]);
			}

			BFrustum bf_projection = BFrustum(cascade_projection);
			bf_projection.Transform(
				mat4::CreateRotationFromDirection(render_camera->GetCameraForward(),
					render_camera->GetCameraRight(), render_camera->GetCameraUp()) *
				mat4::CreateTranslation(render_camera->GetCameraPosition()));
			bf_projection.Transform(m_view_projection_matrix);

			Vec3 max_pos = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
			Vec3 min_pos = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);

			for (const auto& corner : bf_projection.GetCorners())
			{
				for (int j = 0; j < 3; ++j)
				{
					max_pos[j] = max(max_pos[j], corner[j]);
					min_pos[j] = min(min_pos[j], corner[j]);
				}
			}

			Vec3 cascade_center = (max_pos + min_pos) * 0.5f;

			m_cascade_x_offset[i] = -cascade_center.x;
			m_cascade_y_offset[i] = -cascade_center.y;

			Mat4 cascade_offset = mat4::CreateTranslation(Vec3(m_cascade_x_offset[i], m_cascade_y_offset[i], 0.0f));

			m_cascade_scale[i] = 2.0f / max(max_pos.x - min_pos.x, max_pos.y - min_pos.y);

			Mat4 cascade_scale = mat4::CreateScale(m_cascade_scale[i], m_cascade_scale[i], 1.0f);

			m_world_to_cascade_matrix[i] = m_view_projection_matrix * cascade_offset * cascade_scale;
		}
	}
}
