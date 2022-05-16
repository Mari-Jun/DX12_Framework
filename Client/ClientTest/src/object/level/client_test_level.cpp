#include <client/core/core.h>
#include <client/core/log.h>
#include <client/math/math.h>
#include <client/input/input.h>
#include "object/level/client_test_level.h"
#include "object/actor/client_test_actor.h"

namespace client_test
{
	ClientTestLevel::ClientTestLevel(const std::string& name)
		: Level(name)
	{
	}

	bool ClientTestLevel::Initialize()
	{
		LOG_INFO("Initialize {0}", m_name);

		auto player = CreateSPtr<ClientTestActor>(eMobilityState::kMovable, "player actor");
		SpawnActor(player);

		RegisterPressedEvent("spawn actor", { EventKeyInfo{eKey::kJ} },
			[this]()->bool {
				auto actor = CreateSPtr<ClientTestActor>(eMobilityState::kStatic, "actor " + std::to_string(m_actor_count++));
				SpawnActor(actor);
				m_actors.push(actor);
				return true; 
			});

		RegisterPressedEvent("kill actor", { EventKeyInfo{eKey::kK} },
			[this]()->bool {
				if (m_actors.empty() == false)
				{
					m_actors.front()->SetActorState(eActorState::kDead);
					m_actors.pop();
				}
				return true;
			});

		return true;
	}

	void ClientTestLevel::Shutdown()
	{
		LOG_INFO("Shutdown {0}", m_name);
	}

	void ClientTestLevel::Update(float delta_time)
	{
	}
}
