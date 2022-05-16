#include "stdafx.h"
#include "client/object/component/sky/sky_sphere_component.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	SkySphereComponent::SkySphereComponent(const std::string& name)
		: SkyComponent(eSkyType::kSphere, Render::ConvertShaderType(eShaderType::kSky), name)
	{
	}
}
