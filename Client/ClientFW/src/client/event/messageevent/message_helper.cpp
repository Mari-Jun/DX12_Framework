#include "stdafx.h"
#include "client/event/messageevent/message_helper.h"
#include "client/event/messageevent/message_event_manager.h"
#include "client/event/core/event_system.h"

namespace client_fw
{
	void MessageHelper::RegisterMessageEvent(SPtr<MessageEventInfo>&& message)
	{
		EventSystem::GetEventSystem().GetMessageEventManager()->RegisterMessageEvent(std::move(message));
	}

	void MessageHelper::RegisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor)
	{
		EventSystem::GetEventSystem().GetMessageEventManager()->RegisterMessageReceiver(event_id, actor);
	}

	void MessageHelper::UnregisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor)
	{
		EventSystem::GetEventSystem().GetMessageEventManager()->UnregisterMessageReceiver(event_id, actor);
	}
}
