#include <client/core/core.h>
#include <client/core/log.h>
#include <client/math/math.h>
#include <client/input/input.h>
#include "object/actor/client_test_actor.h"
#include "object/component/client_test_component.h"

#include <random>

namespace client_test
{
	ClientTestActor::ClientTestActor(eMobilityState mobility, const std::string& name)
		: Actor(mobility, name)
	{
	}

	bool ClientTestActor::Initialize()
	{
		LOG_INFO("Initialize {0}", m_name);

		switch (m_mobility_state)
		{
		case eMobilityState::kStatic:
			break;
		case eMobilityState::kMovable:
			RegisterAxisEvent("move_forward", { AxisEventKeyInfo{eKey::kW, 1.0f}, AxisEventKeyInfo{eKey::kS, -1.0f} },
				[this](float axis)->bool { Move(std::move(GetForward() * axis)); return true; });
			RegisterAxisEvent("move_right_side", { AxisEventKeyInfo{eKey::kD, 1.0f}, AxisEventKeyInfo{eKey::kA, -1.0f} },
				[this](float axis)->bool { Move(std::move(GetRight() * axis)); return true; });
			RegisterAxisEvent("turn", { AxisEventKeyInfo{eKey::kE, 1.0f}, AxisEventKeyInfo{eKey::kQ, -1.0f} }, 
				[this](float axis)->bool { Turn(axis); return true; });
			RegisterPressedEvent("show_pos", { EventKeyInfo{eKey::kF} },
				[this]()->bool { m_show_pos = !m_show_pos; return true;  });
			RegisterPressedEvent("show_forward_vector", { EventKeyInfo{ eKey::kG } },
				[this]()->bool { m_show_forward = !m_show_forward; return true;  });
			RegisterPressedEvent("attach component", { EventKeyInfo{eKey::kT} },
				[this]()->bool {
					int num = rand() % 100;
					auto component = CreateSPtr<ClientTestComponent>("test component " + std::to_string(num), num);
					AttachComponent(component);
					m_components.push(component);
					return true;  
				});
			RegisterPressedEvent("detach component", { EventKeyInfo{eKey::kY} },
				[this]()->bool {
					if (m_components.empty() == false)
					{
						auto component = m_components.front();
						DetachComponent(component);
						m_components.pop();
					}
					return true;
				});
			break;
		}

		return true;
	}

	void ClientTestActor::Shutdown()
	{
		LOG_INFO("Shutdown {0}", m_name);
	}

	void ClientTestActor::Update(float delta_time)
	{
		if (m_mobility_state == eMobilityState::kMovable)
		{
			if (m_show_forward)
				LOG_TRACE(GetForward());
			if(m_show_pos)
				LOG_TRACE(GetPosition());
		}

	}

	void ClientTestActor::Move(Vec3&& direction)
	{
		auto pos = GetPosition();
		pos += direction;
		SetPosition(pos);
	}

	void ClientTestActor::Turn(float degrees)
	{
		auto rot = GetRotation();
		rot *= quat::CreateQuaternionFromAxis(vec3::AXIS_Y, math::ToRadian(degrees));
		SetRotation(rot);
	}
}