#include "stdafx.h"
#include "client/object/component/util/render_camera_component.h"
#include "client/object/actor/core/actor.h"
#include "client/physics/collision/collisioner/ray_collisioner.h"
#include "client/renderer/core/render.h"
#include "client/asset/texture/texture.h"

namespace client_fw
{
	RenderCameraComponent::RenderCameraComponent(const std::string& name)
		: CameraComponent(eCameraUsage::kBasic, name)
	{
		m_post_processing_info = CreateUPtr<RenderCameraPostProcessingInfo>();
	}

	void RenderCameraComponent::Shutdown()
	{
		for (const auto& [name, texture] : m_rw_textures)
			texture->Shutdown();
		if (m_render_texture != nullptr)
			m_render_texture->Shutdown();
		CameraComponent::Shutdown();
	}

	void RenderCameraComponent::UpdateViewMatrix()
	{
		Vec3 target, up;
		if (m_owner_controller.expired() || (m_owner_controller.expired() == false && m_use_controller_rotation == false))
		{
			m_camera_position = GetWorldPosition();
			m_camera_forward = GetWorldForward();
			target = m_camera_position + m_camera_forward;
			m_camera_up = GetWorldUp();
			m_camera_right = GetWorldRight();
		}
		else
		{
			const auto& owner = m_owner.lock();
			Mat4 world_matrix = mat4::CreateScale(owner->GetScale());
			world_matrix *= mat4::CreateRotationFromQuaternion(m_owner_controller.lock()->GetRotation());
			world_matrix *= mat4::CreateTranslation(owner->GetPosition());

			m_camera_position = vec3::TransformCoord(m_local_position, world_matrix);
			m_camera_forward = m_owner_controller.lock()->GetForward();
			target = m_camera_position + m_camera_forward;
			m_camera_up = m_owner_controller.lock()->GetUp();
			m_camera_right = m_owner_controller.lock()->GetRight();
		}

		m_view_matrix = mat4::LookAt(m_camera_position, target, m_camera_up);

		m_inverse_view_matrix = mat4::Inverse(m_view_matrix);
	}

	void RenderCameraComponent::SetMainCamera()
	{
		Render::SetMainCamera(SharedFromThis());
	}

	void RenderCameraComponent::SetOwnerController(const WPtr<Actor>& owner)
	{
		m_owner_controller = owner;
		UpdateWorldMatrix();
	}

	SPtr<RenderCameraComponent> RenderCameraComponent::SharedFromThis()
	{
		return std::static_pointer_cast<RenderCameraComponent>(shared_from_this());
	}

	SpringArmRenderCameraComponent::SpringArmRenderCameraComponent(const std::string& name)
		: RenderCameraComponent(name)
	{
		if (m_collisioner == nullptr)
		{
			m_oriented_box = CreateSPtr<BOrientedBox>();

			auto collisioner = CreateUPtr<RayCollisioner>();
			collisioner->SetOriginFunction([this]() 
				{
					return GetWorldPosition();
				});
			collisioner->SetDirectionFunction([this]()
				{
					return vec3::Normalize(GetCameraPosition() - GetWorldPosition());
				});
			collisioner->SetDistanceFunction([this](float distance)
				{
					m_distance = min(distance - 15.f, m_distance);
				});
			m_collisioner = std::move(collisioner);
		}
	}

	bool SpringArmRenderCameraComponent::Initialize()
	{
		bool ret = RenderCameraComponent::Initialize();
		return ret;
	}

	void SpringArmRenderCameraComponent::Update(float delta_time)
	{
		RenderCameraComponent::Update(delta_time);

		m_offset = m_spring_speed * delta_time;
	}

	void SpringArmRenderCameraComponent::UpdateViewMatrix()
	{
		Vec3 target, up;
		if (m_owner_controller.expired() || (m_owner_controller.expired() == false && m_use_controller_rotation == false))
		{
			m_camera_forward = GetWorldForward();
			m_camera_position = GetWorldPosition() - m_camera_forward * m_distance;
			target = m_camera_position + m_camera_forward;
			m_camera_up = GetWorldUp();
			m_camera_right = GetWorldRight();
		}
		else
		{
			m_camera_forward = m_owner_controller.lock()->GetForward();
			m_camera_position = GetWorldPosition() - m_camera_forward * m_distance;
			target = GetWorldPosition();
			m_camera_up = m_owner_controller.lock()->GetUp();
			m_camera_right = m_owner_controller.lock()->GetRight();
		}

		m_view_matrix = mat4::LookAt(m_camera_position, target, m_camera_up);

		m_distance = min(m_max_distance, m_distance + m_offset);
		m_offset = 0.0f;


		m_inverse_view_matrix = mat4::Inverse(m_view_matrix);
	}

	void SpringArmRenderCameraComponent::UpdateOrientedBox()
	{
		m_oriented_box->Transform(BOrientedBox(vec3::ZERO, Vec3(10.f, 10.f, 10.f)), GetWorldMatrix());
	}

	void SpringArmRenderCameraComponent::SetMaxDistance(float distance)
	{
		m_max_distance = distance;
		m_distance = min(m_distance, m_max_distance);
	}
}
