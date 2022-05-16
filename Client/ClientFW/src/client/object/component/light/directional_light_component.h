#pragma once
#include "client/object/component/light/core/light_component.h"

namespace client_fw
{
	constexpr static INT s_directional_light_max_texture_size = 8192;

	class RenderCameraComponent;
	class ShadowCascadeCameraComponent;

	struct CascadeShadowForRenderCamera
	{
		WPtr<RenderCameraComponent> render_camera;
		SPtr<ShadowCascadeCameraComponent> shadow_cascade_camera;
	};

	class DirectionalLightComponent : public LightComponent
	{
	public:
		DirectionalLightComponent();
		virtual ~DirectionalLightComponent() = default;

		virtual bool Initialize() override;

	private:
		std::vector<CascadeShadowForRenderCamera> m_cascade_shadows_camera;

	public:
		const std::vector<CascadeShadowForRenderCamera>& GetCascadeShadowsCameras() const { return m_cascade_shadows_camera; }
		void RegisterCascadeAndRenderCamera(const WPtr<RenderCameraComponent>& render_camera_comp,
			const SPtr<ShadowCascadeCameraComponent>& cascade_camera_comp);
		void UnregisterRenderCamera(const WPtr<RenderCameraComponent>& render_camera_comp);

	private:
		virtual void UpdateShadowTextureSize() override;

	};
}


