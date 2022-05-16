#include "stdafx.h"
#include "client/event/packetevent/packet_helper.h"
#include "client/event/packetevent/packet_event_manager.h"
#include "client/event/core/event_system.h"

namespace client_fw
{
	void PacketHelper::RegisterPacketEventToActor(SPtr<MessageEventInfo>&& message, UINT id)
	{
		EventSystem::GetEventSystem().GetPacketEventManager()->RegisterPacketEventToActor(std::move(message), id);
	}

	void PacketHelper::RegisterPacketEventToLevel(SPtr<MessageEventInfo>&& message)
	{
		EventSystem::GetEventSystem().GetPacketEventManager()->RegisterPacketEventToLevel(std::move(message));
	}

	void PacketHelper::RegisterPacketEventToServer(SPtr<MessageEventInfo>&& message)
	{
		EventSystem::GetEventSystem().GetPacketEventManager()->RegisterPacketEventToServer(std::move(message));
	}

	void PacketHelper::ConnectActorToServer(const SPtr<Actor>& actor, UINT id)
	{
		EventSystem::GetEventSystem().GetPacketEventManager()->ConnectActorToServer(actor, id);
	}

	SPtr<Actor> PacketHelper::DisconnectActorFromServer(UINT id)
	{
		return EventSystem::GetEventSystem().GetPacketEventManager()->DisconnectActorFromServer(id);
	}
}
