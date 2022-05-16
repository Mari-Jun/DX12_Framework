#include "stdafx.h"
#include "client/event/messageevent/message_event_manager.h"
#include "client/event/messageevent/message_event_info.h"
#include "client/object/level/core/level_manager.h"
#include "client/object/level/core/level.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	MessageEventManager::MessageEventManager()
	{
	}

	MessageEventManager::~MessageEventManager()
	{
	}

	void MessageEventManager::ExecuteEvent()
	{
		const auto& current_level = LevelManager::GetLevelManager().GetCurrentLevel();
		if (current_level != nullptr)
		{
			while (m_message_queue.empty() == false)
			{
				const auto& message = m_message_queue.front();
				const auto& event_id = message->GetEventID();

				for (auto iter = m_message_receive_actors.lower_bound(event_id);
					iter != m_message_receive_actors.upper_bound(event_id); ++iter)
				{
					iter->second->ExecuteMessage(message);
				}

				current_level->ExecuteMessage(message);

				m_message_queue.pop();
			}
		}
	}

	void MessageEventManager::RegisterMessageEvent(SPtr<MessageEventInfo>&& message)
	{
		m_message_queue.emplace(std::move(message));
	}

	void MessageEventManager::RegisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor)
	{
		m_message_receive_actors.emplace(event_id, actor);
	}

	void MessageEventManager::UnregisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor)
	{
		for (auto iter = m_message_receive_actors.lower_bound(event_id);
			iter != m_message_receive_actors.upper_bound(event_id); ++iter)
		{
			if (iter->second == actor)
			{
				iter = m_message_receive_actors.erase(iter);
				break;
			}
		}
	}
}
