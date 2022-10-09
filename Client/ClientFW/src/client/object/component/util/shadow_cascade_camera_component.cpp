#include "stdafx.h"
#include "client/object/component/util/shadow_cascade_camera_component.h"
#include "client/object/component/util/render_camera_component.h"
#include "client/object/actor/core/actor.h"
#include "client/asset/texture/texture.h"

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

	void ShadowCascadeCameraComponent::Shutdown()
	{
		if (m_shadow_array_texture != nullptr)
			m_shadow_array_texture->Shutdown();
		CameraComponent::Shutdown();
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

			const Vec3& cam_pos = render_camera->GetCameraPosition();
			const Vec3& cam_forward = render_camera->GetCameraForward();
			const Vec3& cam_right = render_camera->GetCameraRight();
			const Vec3& cam_up = render_camera->GetCameraUp();

			const float tan_fovx = tanf(render_camera->GetAspectRatio() * math::ToRadian(render_camera->GetFieldOfView()));
			const float tan_fovy = tanf(render_camera->GetAspectRatio());

			Vec3 sphere_center = cam_pos + cam_forward * (near_z + 0.5f * (near_z + far_z));
			const Vec3 bound_span = cam_pos + (cam_right * -tan_fovx + cam_up * tan_fovy + cam_forward) * far_z - sphere_center;
			float radius = bound_span.Length();

			m_cascade_bounding_sphere = BSphere(sphere_center, radius);

			// get projectoin matrix from bounding sphere
			m_projection_matrix = mat4::Ortho(radius, radius, -radius * 0.5f, radius * 0.5f);

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
			float near_z, far_z;

			if (i == 0)
			{
				near_z = render_camera->GetNearZ();
				far_z = m_cascade_far_z * m_cascade_level_ratio[i];
			}
			else
			{
				near_z = m_cascade_far_z * m_cascade_level_ratio[i - 1];
				far_z = m_cascade_far_z * m_cascade_level_ratio[i];
			}


			std::array<Vec3, 8> frustum_points;

			const Vec3& cam_pos = render_camera->GetCameraPosition();
			const Vec3& cam_forward = render_camera->GetCameraForward();
			const Vec3& cam_right = render_camera->GetCameraRight();
			const Vec3& cam_up = render_camera->GetCameraUp();

			const float tan_fovx = tanf(render_camera->GetAspectRatio() * math::ToRadian(render_camera->GetFieldOfView()));
			const float tan_fovy = tanf(render_camera->GetAspectRatio());

			frustum_points[0] = cam_pos + (cam_right * -tan_fovx + cam_up * tan_fovy + cam_forward) * near_z;
			frustum_points[1] = cam_pos + (cam_right * tan_fovx + cam_up * tan_fovy + cam_forward) * near_z;
			frustum_points[2] = cam_pos + (cam_right * tan_fovx - cam_up * tan_fovy + cam_forward) * near_z;
			frustum_points[3] = cam_pos + (cam_right * -tan_fovx - cam_up * tan_fovy + cam_forward) * near_z;

			frustum_points[4] = cam_pos + (cam_right * -tan_fovx + cam_up * tan_fovy + cam_forward) * far_z;
			frustum_points[5] = cam_pos + (cam_right * tan_fovx + cam_up * tan_fovy + cam_forward) * far_z;
			frustum_points[6] = cam_pos + (cam_right * tan_fovx - cam_up * tan_fovy + cam_forward) * far_z;
			frustum_points[7] = cam_pos + (cam_right * -tan_fovx - cam_up * tan_fovy + cam_forward) * far_z;

			Vec3 max_pos = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
			Vec3 min_pos = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);

			for (auto& point : frustum_points)
			{
				point.TransformCoord(m_view_projection_matrix);

				for (int j = 0; j < 3; ++j)
				{
					max_pos[j] = max(max_pos[j], point[j]);
					min_pos[j] = min(min_pos[j], point[j]);
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
