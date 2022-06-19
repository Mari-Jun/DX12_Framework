#pragma once
#include "client/object/component/util/core/camera_component.h"

namespace client_fw
{
	struct RenderCameraPostProcessingInfo
	{
		bool use_post_processing = true;
		bool use_blur = false;
		float blur_sigma = 2.5f;
		int blur_count = 4;
		bool use_sobel_edge = false;
	};

	class RenderTexture;
	class RWTexture;

	class RenderCameraComponent : public CameraComponent
	{
	public:
		RenderCameraComponent(const std::string& name = "render camera component");
		virtual ~RenderCameraComponent() = default;

		virtual void UpdateViewMatrix() override;

	protected:
		WPtr<Actor> m_owner_controller;
		bool m_use_controller_rotation = false;
		Vec3 m_camera_right;
		Vec3 m_camera_up;

	public:
		void SetMainCamera();
		void SetOwnerController(const WPtr<Actor>& owner);
		bool IsUseControllerRotation() const { return m_use_controller_rotation; }
		void UseControllerRotation(bool use) { m_use_controller_rotation = use; }
		const Vec3& GetCameraRight() const { return m_camera_right; }
		const Vec3& GetCameraUp() const { return m_camera_up; }

	private:
		SPtr<RenderTexture> m_render_texture;
		std::map<std::string, SPtr<RWTexture>> m_rw_textures;

	public:
		const SPtr<RenderTexture>& GetRenderTexture() const { return m_render_texture; }
		// 카메라가 생성되면 카메라가 보는 장면을 그릴 Texture가 필요한데, 그 Texture를 뜻한다. 
		// 사용자가 직접적으로 호출할 필요는 없다.
		void SetRenderTexture(const SPtr<RenderTexture>& texture) { m_render_texture = texture; }
		
		const std::map<std::string, SPtr<RWTexture>>& GetRWTextures() const { return m_rw_textures; }
		const SPtr<RWTexture>& GetRWTexture(const std::string& name) const { return m_rw_textures.at(name); }
		// 계산 셰이더 등에서 사용되는 RW Texture입니다.
		// 사용자가 직접적으로 Set함수를 Shader에서 호출하면 됩니다.
		void SetRWTexture(const std::string& name, const SPtr<RWTexture>& texture) { m_rw_textures[name] = texture; }

	protected:
		UPtr<RenderCameraPostProcessingInfo> m_post_processing_info;

	public:
		const UPtr<RenderCameraPostProcessingInfo>& GetPostProcessingInfo() const { return m_post_processing_info; }

	protected:
		SPtr<RenderCameraComponent> SharedFromThis();
	};

	class SpringArmRenderCameraComponent : public RenderCameraComponent
	{
	public:
		SpringArmRenderCameraComponent(const std::string& name = "spring arm render camera component");
		virtual ~SpringArmRenderCameraComponent() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;

		virtual void UpdateViewMatrix() override;

	protected:
		virtual void UpdateOrientedBox() override;

	private:
		Vec3 m_spring_arm_target_position;
		float m_max_distance = 400.0f;
		float m_distance = 400.0f;
		float m_offset = 0.0f;
		float m_spring_speed = 400.0f;

	public:
		const Vec3& GetSpringArmTargetPosition() const { return m_spring_arm_target_position; }
		void SetSpringArmTargetPosition(const Vec3& position) { m_spring_arm_target_position = position; }
		float GetMaxDistance() const { return m_max_distance; }
		void SetMaxDistance(float distance);
		float GetDistance() const { return m_distance; }
		float GetSpringSpeed() const { return m_spring_speed; }
		void SetSpringSpeed(float speed) { m_spring_speed = speed; }
	};
}


