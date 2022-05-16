#pragma once
#include <client/object/actor/core/actor.h>
#include <queue>

namespace client_test
{
	using namespace client_fw;

	class ClientTestActor final : public Actor
	{
	public:
		ClientTestActor(eMobilityState mobility, const std::string& name = "Actor");
		virtual ~ClientTestActor() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;

	private:
		void Move(Vec3&& direction);
		void Turn(float degrees);
		bool m_show_pos = false;
		bool m_show_forward = false;
		
		std::queue<SPtr<class Component>> m_components;
		
	};
}



