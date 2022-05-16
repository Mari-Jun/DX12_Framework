#include "stdafx.h"
#include "client/object/component/render/sphere_component.h"
#include "client/physics/collision/collisioner/sphere_collisioner.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	SphereComponent::SphereComponent(float radius, const std::string& name)
		: ShapeComponent(name, Vec3(radius, radius, radius)
		, Render::ConvertShaderType(eShaderType::kShapeBox))
	{
		if (m_collisioner == nullptr)
			m_collisioner = CreateUPtr<SphereCollisioner>();
	}

	SPtr<SphereComponent> SphereComponent::SharedFromThis()
	{
		return std::static_pointer_cast<SphereComponent>(shared_from_this());
	}
}
