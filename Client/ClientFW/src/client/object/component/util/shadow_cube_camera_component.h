#pragma once
#include "client/object/component/util/core/camera_component.h"

namespace client_fw
{
	class ShadowCubeTexture;

	class ShadowCubeCameraComponent : public CameraComponent
	{
	public:
		ShadowCubeCameraComponent(const std::string& name = "shadow cube camera component");
		virtual ~ShadowCubeCameraComponent() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
		virtual void UpdateViewMatrix() override;
		virtual void UpdateProjectionMatrix() override;

	private:
		SPtr<ShadowCubeTexture> m_shadow_cube_texture;
		SPtr<ShadowCubeTexture> m_static_shadow_cube_texture;
		std::array<Mat4, 6> m_cube_view_matrixs;

	public:
		const SPtr<ShadowCubeTexture>& GetShadowCubeTexture() const { return m_shadow_cube_texture; }
		const SPtr<ShadowCubeTexture>& GetStaticShadowCubeTexture() const { return m_static_shadow_cube_texture; }
		// 카메라가 생성되면 카메라가 보는 장면을 그릴 Texture가 필요한데, 그 Texture를 뜻한다. 
		// 사용자가 직접적으로 호출할 필요는 없다.
		void SetShadowCubeTexture(const SPtr<ShadowCubeTexture>& texture) { m_shadow_cube_texture = texture; }
		void SetStaticShadowCubeTexture(const SPtr<ShadowCubeTexture>& texture) { m_static_shadow_cube_texture = texture; }

		const std::array<Mat4, 6>& GetCubeViewMatrixs() const { return m_cube_view_matrixs; }
	};


}
