#include <include/client_core.h>
#include <client/object/component/render/texture_billboard_component.h>

#include "object/actor/billboard_actor.h"

namespace simulation
{
	TextureBillboardActor::TextureBillboardActor(eMobilityState mobility)
		: Actor(mobility, "Billboard")
	{
		m_billboard_component = CreateSPtr<TextureBillboardComponent>();
	}

	bool TextureBillboardActor::Initialize()
	{
		return AttachComponent(m_billboard_component);
	}
}
