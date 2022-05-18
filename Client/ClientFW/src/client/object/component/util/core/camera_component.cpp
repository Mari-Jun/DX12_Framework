#include "stdafx.h"
#include "client/object/component/util/core/camera_component.h"
#include "client/object/component/core/render_component.h"
#include "client/object/actor/core/actor.h"
#include "client/renderer/core/render.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	CameraComponent::CameraComponent(eCameraUsage usage, const std::string& name)
		: SceneComponent(name), m_camera_state(eCameraState::kPaused)
		, m_camera_usage(usage), m_projection_mode(eProjectionMode::kPerspective)
	{
	}

	bool CameraComponent::Initialize()
	{
		return RegisterToRenderSystem();
	}

	void CameraComponent::Shutdown()
	{
		UnregisterFromRenderSystem();
	}

	void CameraComponent::Update(float delta_time)
	{
		if (m_is_updated_viewport)
		{
			SetAspectRatio(static_cast<float>(m_viewport.width) / static_cast<float>(m_viewport.height));
			UpdateProjectionMatrix();

			m_view_projection_matrix = m_view_matrix * m_projection_matrix;

			m_require_resize_texture = true;
			m_is_updated_viewport = false;
		}
	}

	void CameraComponent::UpdateWorldMatrix()
	{
		SceneComponent::UpdateWorldMatrix();

		UpdateViewMatrix();

		m_view_projection_matrix = m_view_matrix * m_projection_matrix;

		m_bounding_frustum.Transform(m_bf_projection, m_inverse_view_matrix);
	}

	void CameraComponent::UpdateViewport(LONG left, LONG top, LONG width, LONG height)
	{
		m_viewport.left = left;
		m_viewport.width = width;
		m_viewport.top = top;
		m_viewport.height = height;
		m_is_updated_viewport = true;
	}

	void CameraComponent::UpdateViewMatrix()
	{
		Vec3 target, up;

		m_camera_position = GetWorldPosition();
		m_camera_forward = GetWorldForward();
		target = m_camera_position + m_camera_forward;
		up = GetWorldUp();

		m_view_matrix = mat4::LookAt(m_camera_position, target, up);

		m_inverse_view_matrix = mat4::Inverse(m_view_matrix);
	}

	void CameraComponent::UpdateProjectionMatrix()
	{
		switch (m_projection_mode)
		{
		case client_fw::eProjectionMode::kPerspective:
			m_projection_matrix = GetPerspectiveMatrix();
			break;
		case client_fw::eProjectionMode::kOrthographic:
			m_projection_matrix = GetOrthoMatrix();
			break;
		}
		m_bf_projection = BFrustum(m_projection_matrix);
		m_bounding_frustum.Transform(m_bf_projection, m_inverse_view_matrix);
	}

	bool CameraComponent::RegisterToRenderSystem()
	{
		if (Render::RegisterCameraComponent(SharedFromThis()))
			return true;
		return false;
	}

	void CameraComponent::UnregisterFromRenderSystem()
	{
		Render::UnregisterCameraComponent(SharedFromThis());
	}



	Mat4 CameraComponent::GetPerspectiveMatrix() const
	{
		return mat4::Perspective(math::ToRadian(m_field_of_view), m_aspect_ratio, m_near_z, m_far_z);
	}

	Mat4 CameraComponent::GetOrthoMatrix() const
	{
		return  mat4::Ortho(static_cast<float>(m_viewport.left),
			static_cast<float>(m_viewport.left + m_viewport.width),
			static_cast<float>(m_viewport.top + m_viewport.height),
			static_cast<float>(m_viewport.top), m_near_z, m_far_z);
	}

	void CameraComponent::SetAspectRatio(float aspect_ratio)
	{
		m_aspect_ratio = aspect_ratio;
		m_is_updated_viewport = true;
	}

	void CameraComponent::SetFieldOfView(float fov)
	{
		m_field_of_view = fov;
		m_is_updated_viewport = true;
	}

	void CameraComponent::SetNearZ(float near_z)
	{
		m_near_z = near_z;
		m_is_updated_viewport = true;
	}

	void CameraComponent::SetFarZ(float far_z)
	{
		m_far_z = far_z;
		m_is_updated_viewport = true;
	}

	void CameraComponent::UpdateDestructibleRenderComponentVisibility(const std::function<bool(const SPtr<RenderComponent>)>& trigger_function)
	{
		int count = 0;

		for (auto iter = m_destructible_render_comps.rbegin(); iter != m_destructible_render_comps.rend(); ++iter)
		{
			if (iter->expired() == false)
			{
				const auto& render_comp = iter->lock();
				if (trigger_function(render_comp))
				{
					render_comp->SetVisiblity(true);
					render_comp->UpdateLevelOfDetail(GetCameraPosition());
				}
			}
			else
			{
				std::iter_swap(iter, m_destructible_render_comps.rbegin() + count);
				++count;
			}
		}

		while (count--)
			m_destructible_render_comps.pop_back();
	}

	SPtr<CameraComponent> CameraComponent::SharedFromThis()
	{
		return std::static_pointer_cast<CameraComponent>(shared_from_this());
	}
}
