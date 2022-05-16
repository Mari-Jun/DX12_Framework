#include "stdafx.h"
#include "client/input/input.h"
#include "client/event/inputevent/input_event_info.h"
#include "client/event/inputevent/input_event_manager.h"

namespace client_fw
{
	InputEventManager::InputEventManager()
		: m_input_mode(eInputMode::kGameOnly)
	{
	}
	
	InputEventManager::~InputEventManager()
	{
	}

	void InputEventManager::ExecuteEvent()
	{
		ExecuteEvents(m_actor_events, { eInputMode::kGameOnly });
		ExecuteEvents(m_player_controller_events, { eInputMode::kGameOnly });
		ExecuteEvents(m_level_events, { eInputMode::kGameOnly });
		ExecuteEvents(m_pawn_events, { eInputMode::kGameOnly });
		ExecuteEvents(m_application_events, { eInputMode::kUIOnly, eInputMode::kGameOnly });
	}

	void InputEventManager::ExecuteEvents(const std::vector<UPtr<InputEventInfo>>& events, std::vector<eInputMode>&& modes) const
	{
		if (m_input_mode == eInputMode::kUIAndGame ||
			std::any_of(modes.cbegin(), modes.cend(), [this](eInputMode mode) { return mode == m_input_mode; }))
		{
			for (const auto& event : events)
				event->ExecuteEvent();
		}
	}

	void InputEventManager::DeleteEvent(std::vector<UPtr<InputEventInfo>>& events, const std::string& name)
	{
		auto delete_event = std::find_if(events.begin(), events.end(), [name](const UPtr<InputEventInfo>& event)
			{ return name == event->GetEventName(); });
		if (delete_event != events.cend())
		{
			std::iter_swap(delete_event, events.end() - 1);
			events.pop_back();
		}
	}

	bool InputEventManager::RegisterEvent(UPtr<InputEventInfo>&& event_info, eInputOwnerType type)
	{
		const std::string& event_name = event_info->GetEventName();

		if (m_event_names.find(event_name) == m_event_names.cend())
		{
			switch (type)
			{
			case eInputOwnerType::kApplication:
				m_application_events.emplace_back(std::move(event_info));
				break;
			case eInputOwnerType::kLevel:
				m_level_events.emplace_back(std::move(event_info));
				break;
			case eInputOwnerType::kActor:
				m_actor_events.emplace_back(std::move(event_info));
				break;
			case eInputOwnerType::kPawn:
				m_pawn_events.emplace_back(std::move(event_info));
				break;
			case eInputOwnerType::kPlayerController:
				m_player_controller_events.emplace_back(std::move(event_info));
				break;
			default:
				break;
			}
			m_event_names.emplace(event_name, type);
			return true;
		}
		else
		{
			LOG_WARN("Event \"{0}\" is already exist", event_name);
			return false;
		}
	}

	void InputEventManager::UnregisterEvent(const std::string& name)
	{
		auto iter = m_event_names.find(name);
		if (iter !=  m_event_names.cend())
		{
			auto [name, type] = *iter;
			switch (type)
			{
			case eInputOwnerType::kApplication:
				break;
			case eInputOwnerType::kLevel:
				DeleteEvent(m_level_events, name);
				break;
			case eInputOwnerType::kActor:
				DeleteEvent(m_actor_events, name);
				break;
			case eInputOwnerType::kPawn:
				DeleteEvent(m_pawn_events, name);
				break;
			case eInputOwnerType::kPlayerController:
				DeleteEvent(m_player_controller_events, name);
			default:
				break;
			}
			m_event_names.erase(iter);
		}
		else
		{
			LOG_WARN("Event \"{0}\" is already deleted", name);
		}
	}
}
