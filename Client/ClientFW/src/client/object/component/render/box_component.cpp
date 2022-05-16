#include "stdafx.h"
#include "client/object/component/render/box_component.h"
#include "client/renderer/core/render.h"
#include "client/physics/collision/collisioner/box_collisioner.h"

namespace client_fw
{
	BoxComponent::BoxComponent(const Vec3& extents, const std::string& name)
		: ShapeComponent(name, extents, Render::ConvertShaderType(eShaderType::kShapeBox))
	{
		if (m_collisioner == nullptr)
			m_collisioner = CreateUPtr<BoxCollisioner>();
	}

	SPtr<BoxComponent> BoxComponent::SharedFromThis()
	{
		return std::static_pointer_cast<BoxComponent>(shared_from_this());
	}
}