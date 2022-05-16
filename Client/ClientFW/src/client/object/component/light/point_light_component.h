#pragma once
#include "client/object/component/light/core/local_light_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	constexpr static INT s_point_light_max_texture_size = 2048;

	class ShadowCubeCameraComponent;

	class PointLightComponent : public LocalLightComponent
	{
	public:
		PointLightComponent(const std::string& name = "point light component",
			const std::string& draw_shader_name = Render::ConvertShaderType(eShaderType::kPointLight));
		virtual ~PointLightComponent() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	private:
		virtual void UpdateWorldMatrix() override;
		virtual void UpdateLocalMatrix() override;
		virtual void UpdateOrientedBox() override;
		virtual void SetVisiblity(bool value) override;

	private:
		SPtr<ShadowCubeCameraComponent> m_shadow_cube_camera;

	private:
		virtual void UpdateShadowTextureSize() override;
		virtual void UpdateShadowCameraProjection() override;

	public:
		const SPtr<ShadowCubeCameraComponent>& GetShadowCubeCamera() const { return m_shadow_cube_camera; }
	};
}