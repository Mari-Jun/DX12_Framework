#include <include/client_core.h>
#include <client/object/component/render/material_billboard_component.h>
#include "material_billboard_actor.h"

namespace event_test
{
	MaterialBillboardActor::MaterialBillboardActor(eMobilityState mobility,
		const std::string& path, const std::string& mtl_name, 
		Vec2&& size, bool fix_up)
		: Actor(mobility, "Billboard"), m_path(path), m_mtl_name(mtl_name)
		, m_size(size), m_fix_up(fix_up)
	{
		m_billboard_component = CreateSPtr<MaterialBillboardComponent>();
	}

	bool MaterialBillboardActor::Initialize()
	{
		m_billboard_component->SetMaterial(m_path, m_mtl_name);
		m_billboard_component->SetSize(m_size);
		m_billboard_component->SetFixUpVector(m_fix_up);
		return AttachComponent(m_billboard_component);
	}
}
