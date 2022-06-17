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
		float m_animation_time = 0.0f;

	public:
		const SPtr<TextureBillboardComponent>& GetTextureBillboardComponent() const { return m_billboard_component; }
		float GetAnimationTime() const { return m_animation_time; }
		void SetAnimationTime(float time) { m_animation_time = time; }
	};
}



