#include "stdafx.h"
#include "client/object/actor/pawn.h"
#include "client/object/actor/controller.h"
#include "client/object/actor/player_controller.h"
#include "client/input/input.h"

namespace client_fw
{
	Pawn::Pawn(const std::string& name)
		: Actor(eMobilityState::kMovable, name)
	{
	}

	void Pawn::Update(float delta_time)
	{
	}

	void Pawn::Possess(const SPtr<Controller>& controller)
	{
		controller->Possess(std::static_pointer_cast<Pawn>(shared_from_this()));
	}

	void Pawn::Possess(const SPtr<PlayerController>& controller)
	{
		controller->Possess(std::static_pointer_cast<Pawn>(shared_from_this()));
	}

	void Pawn::RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys, 
		const std::function<bool()>& func, bool consumption)
	{
		std::string event_name = m_name + " : " + name;
		if (Input::RegisterPressedEvent(event_name, std::move(keys), func, consumption, eInputOwnerType::kPawn))
			RegisterInputEvent(event_name);
	}

	void Pawn::RegisterReleasedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption)
	{
		std::string event_name = m_name + " : " + name;
		if (Input::RegisterReleasedEvent(event_name, std::move(keys), func, consumption, eInputOwnerType::kPawn))
			RegisterInputEvent(event_name);
	}

	void Pawn::RegisterAxisEvent(const std::string& name, std::vector<AxisEventKeyInfo>&& keys, 
		const std::function<bool(float)>& func, bool consumption)
	{
		std::string event_name = m_name + " : " + name;
		if (Input::RegisterAxisEvent(event_name, std::move(keys), func, consumption, eInputOwnerType::kPawn))
			RegisterInputEvent(event_name);
	}

	bool Pawn::AddControllerPitchInput(float value)
	{
		if (m_controller.expired() == false)
		{
			const auto& player_controller = std::dynamic_pointer_cast<PlayerController>(m_controller.lock());
			player_controller->AddPitchInput(value);
			return true;
		}
		return false;
	}

	bool Pawn::AddControllerYawInput(float value)
	{
		if (m_controller.expired() == false)
		{
			const auto& player_controller = std::dynamic_pointer_cast<PlayerController>(m_controller.lock());
			player_controller->AddYawInput(value);
			return true;
		}
		return false;
	}

	bool Pawn::AddControllerRollInput(float value)
	{
		if (m_controller.expired() == false)
		{
			const auto& player_controller = std::dynamic_pointer_cast<PlayerController>(m_controller.lock());
			player_controller->AddRollInput(value);
			return true;
		}
		return false;
	}

	void Pawn::SetController(const SPtr<Controller>& controller)
	{
		m_controller = controller;
	}
	
}
