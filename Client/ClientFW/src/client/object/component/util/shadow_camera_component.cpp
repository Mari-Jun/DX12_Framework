#include "stdafx.h"
#include "client/object/component/util/shadow_camera_component.h"

namespace client_fw
{
	ShadowCameraComponent::ShadowCameraComponent(const std::string& name)
		: CameraComponent(eCameraUsage::kShadow, name)
	{
	}
}
