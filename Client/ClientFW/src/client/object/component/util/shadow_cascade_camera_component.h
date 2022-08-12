#pragma once
#include "client/object/component/util/core/camera_component.h"
#include "client/renderer/core/shadow_camera_manager.h"

namespace client_fw
{
	class ShadowArrayTexture;
	class RenderCameraComponent;

	class ShadowCascadeCameraComponent : public CameraComponent
	{
	public:
		ShadowCascadeCameraComponent(const std::string& name = "shadow cascade camera component");
		virtual ~ShadowCascadeCameraComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;
		virtual void UpdateViewMatrix() override;
		virtual void UpdateProjectionMatrix() override;

	private:
		void UpdateCascadeMatrix();

	private:
		float m_max_cascade_far_z = 5000.f;
		float m_cascade_far_z = 5000.f;
		std::array<float, s_max_cascade_level> m_cascade_level_ratio = { 0.1f, 0.5f, 1.0f };
		std::array<float, s_max_cascade_level> m_cascade_x_offset = { 0.0f, 0.0f, 0.0f };
		std::array<float, s_max_cascade_level> m_cascade_y_offset = { 0.0f, 0.0f, 0.0f };
		std::array<float, s_max_cascade_level> m_cascade_scale = { 1.0f, 1.0f, 1.0f };
		std::array<Mat4, s_max_cascade_level> m_world_to_cascade_matrix;
		BSphere m_cascade_bounding_sphere;

	public:
		float GetMaxCascadeFarZ() const { return m_max_cascade_far_z; }
		void SetMaxCascadeFarZ(float far_z) { m_max_cascade_far_z = far_z; }

		const std::array<float, s_max_cascade_level>& GetCascadeXOffset() const { return m_cascade_x_offset; }
		const std::array<float, s_max_cascade_level>& GetCascadeYOffset() const { return m_cascade_y_offset; }
		const std::array<float, s_max_cascade_level>& GetCascadeScale() const { return m_cascade_scale; }
		const std::array<Mat4, s_max_cascade_level>& GetWorldToCascadeMatrix() const { return m_world_to_cascade_matrix; }

		const BSphere& GetCascadeBoundingSphere() const { return m_cascade_bounding_sphere; }

	private:
		SPtr<ShadowArrayTexture> m_shadow_array_texture;
		WPtr<RenderCameraComponent> m_render_camera_component;

	public:
		const SPtr<ShadowArrayTexture>& GetShadowArrayTexture() const { return m_shadow_array_texture; }
		// 카메라가 생성되면 카메라가 보는 장면을 그릴 Texture가 필요한데, 그 Texture를 뜻한다. 
		// 사용자가 직접적으로 호출할 필요는 없다.
		void SetShadowArrayTexture(const SPtr<ShadowArrayTexture>& texture) { m_shadow_array_texture = texture; }

		const WPtr<RenderCameraComponent>& GetRenderCamera() const { return m_render_camera_component; }
		// Cascade Camera는 기본적으로 Render Camera의 View Frustum을 통해 동작하기 때문에 
		// 그릴 Render Camera가 필요하다.
		// 사용자가 직접적으로 호출할 필요는 없다.
		void SetRenderCamera(const WPtr<RenderCameraComponent>& render_camera) { m_render_camera_component = render_camera; }
	};
}



