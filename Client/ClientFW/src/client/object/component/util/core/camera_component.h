#pragma once
#include "client/object/component/core/scene_component.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	class RenderComponent;

	enum class eProjectionMode
	{
		kPerspective, kOrthographic
	};

	enum class eCameraState
	{
		kActive, kPaused
	};

	enum class eCameraUsage
	{
		kBasic, kShadow, kShadowCube, kShadowCascade
	};

	//카메라가 그리는 크기 (RenderTexture의 Size와도 같다.)
	struct Viewport
	{
		LONG left = 0;
		LONG top = 0;
		LONG width = 100;
		LONG height = 100;
	};

	class CameraComponent : public SceneComponent
	{
	protected:
		CameraComponent(eCameraUsage usage, 
			const std::string& name = "camera component");
		virtual ~CameraComponent() = default;

	public:
		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

		virtual void UpdateWorldMatrix() override;
		virtual void UpdateViewport(LONG left, LONG top, LONG width, LONG height);
		virtual void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix();

	private:
		bool RegisterToRenderSystem();
		void UnregisterFromRenderSystem();

	protected:
		eCameraState m_camera_state;
		eCameraUsage m_camera_usage;
		eProjectionMode m_projection_mode;
		Viewport m_viewport;
		bool m_is_updated_viewport = true;
		bool m_require_resize_texture = false;
		Vec3 m_camera_position;
		Vec3 m_camera_forward;
		Mat4 m_view_matrix;
		Mat4 m_inverse_view_matrix;
		Mat4 m_projection_matrix;
		Mat4 m_view_projection_matrix;
		float m_aspect_ratio = 1.777778f;
		float m_field_of_view = 60.0f;
		float m_near_z = 25.f;
		float m_far_z = 100000.f;
		BFrustum m_bf_projection;
		BFrustum m_bounding_frustum;

	public:
		eCameraState GetCameraState() const { return m_camera_state; }
		void SetActive() { m_camera_state = eCameraState::kActive; }
		void SetPaused() { m_camera_state = eCameraState::kPaused; }
		eProjectionMode GetProjectionMode() const { return m_projection_mode; }
		void SetProjectionMode(eProjectionMode mode) { m_projection_mode = mode; }
		eCameraUsage GetCameraUsage() const { return m_camera_usage; }
		const Viewport& GetViewport() const { return m_viewport; }
		void SetViewport(const Viewport& viewport) { m_viewport = viewport; m_is_updated_viewport = true; }
		bool RequireResizeTexture() const { return m_require_resize_texture; }
		void SetRequireResizeTexture(bool value) { m_require_resize_texture = value; }
		const Vec3& GetCameraPosition() const { return m_camera_position; }
		const Vec3& GetCameraForward() const { return m_camera_forward; }
		const Mat4& GetViewMatrix() const { return m_view_matrix; }
		const Mat4& GetInverseViewMatrix() const { return m_inverse_view_matrix; }
		const Mat4& GetProjectionMatrix() const { return m_projection_matrix; }
		const Mat4& GetViewProjectionMatrix() const { return m_view_projection_matrix; }
		Mat4 GetPerspectiveMatrix() const;
		Mat4 GetOrthoMatrix() const;
		float GetAspectRatio() const { return m_aspect_ratio; }
		float GetFieldOfView() const { return m_field_of_view; }
		float GetNearZ() const { return m_near_z; }
		float GetFarZ() const { return m_far_z; }
		void SetAspectRatio(float aspect_ratio); 
		void SetFieldOfView(float fov);
		void SetNearZ(float near_z);
		void SetFarZ(float far_z);
		const BFrustum& GetBoudingFrustum() const { return m_bounding_frustum; }

	protected:
		std::vector<WPtr<RenderComponent>> m_destructible_render_comps;

	public:
		//사용자 호출 X
		void RegisterDestructibleRenderComponent(const WPtr<RenderComponent>& render_comp) { m_destructible_render_comps.push_back(render_comp); }
		void UpdateDestructibleRenderComponentVisibility(const std::function<bool(const SPtr<RenderComponent>)>& trigger_function);

	protected:
		SPtr<CameraComponent> SharedFromThis();

	};
}



