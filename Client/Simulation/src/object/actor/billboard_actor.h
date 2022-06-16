#pragma once
#include <client/object/actor/core/actor.h>

namespace client_fw
{
	class TextureBillboardComponent;
}

namespace simulation
{
	using namespace client_fw;

	class TextureBillboardActor final : public Actor
	{
	public:
		TextureBillboardActor(eMobilityState mobility);
		virtual ~TextureBillboardActor() = default;

		virtual bool Initialize() override;

	private:
		SPtr<TextureBillboardComponent> m_billboard_component;

	public:
		const SPtr<TextureBillboardComponent>& GetTextureBillboardComponent() const { return m_billboard_component; }
	};
}



