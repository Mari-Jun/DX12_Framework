#include "stdafx.h"
#include "client/object/component/util/shadow_camera_component.h"
#include "client/asset/texture/texture.h"

namespace client_fw
{
	ShadowCameraComponent::ShadowCameraComponent(const std::string& name)
		: CameraComponent(eCameraUsage::kShadow, name)
	{
	}

	void ShadowCameraComponent::Shutdown()
	{
		if (m_shadow_texture != nullptr)
			m_shadow_texture->Shutdown();
		if (m_static_shadow_texture != nullptr)
			m_static_shadow_texture->Shutdown();
		CameraComponent::Shutdown();
	}
}
