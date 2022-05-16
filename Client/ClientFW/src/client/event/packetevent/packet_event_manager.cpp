#include "stdafx.h"
#include "client/event/messageevent/message_event_info.h"
#include "client/event/packetevent/packet_event_manager.h"
#include "client/object/level/core/level_manager.h"
#include "client/object/level/core/level.h"
#include "client/object/level/gamemode/game_mode_base.h"
#include "client/object/actor/core/actor.h"
#include "client/object/actor/pawn.h"
#include "client/core/application.h"
#include "client/event/packetevent/packet_helper.h"

namespace client_fw
{
	PacketEventManager::PacketEventManager()
	{
	}

	PacketEventManager::~PacketEventManager()
	{
	}

	void PacketEventManager::ExecuteEventReceivedFromServer()
	{
		const auto& app = Application::GetApplication();
		const auto& current_level = LevelManager::GetLevelManager().GetCurrentLevel();
		if (current_level != nullptr)
		{
			SPtr<MessageEventInfo> message;
			while (m_server_send_message_to_level_queue.empty() == false)
			{
				if (m_server_send_message_to_level_queue.try_pop(message))
					current_level->ExecuteMessageFromServer(message);
			}

			PacketIDEventInfo actor_message;
			while (m_server_send_message_to_actor_queue.empty() == false)
			{
				if (m_server_send_message_to_actor_queue.try_pop(actor_message) &&
					m_connected_actor_map[actor_message.id] != nullptr)
				{
					m_connected_actor_map[actor_message.id]->ExecuteMessageFromServer(actor_message.message);
				}
			}
		}
	}

	void PacketEventManager::SendEventToServer()
	{
		while (m_server_receive_message_queue.empty() == false)
		{
			SPtr<MessageEventInfo> message;
			if (m_server_receive_message_queue.try_pop(message))
			{
				//이벤트
			}
		}

		const auto& current_level = LevelManager::GetLevelManager().GetCurrentLevel();
		if (current_level != nullptr)
		{
			SPtr<MessageEventInfo>move_message;
			const auto& pawn = current_level->GetGameMode()->GetDefaultPawn();
			if (pawn != nullptr && pawn->IsUpdatedWorldMatrix() && pawn->IsConnectedToServer())
			{
				//여기서 플레이어 좌표랑 rotation 보내주기
				//LOG_INFO(pawn->GetPosition());
			}
		}
	}

	void PacketEventManager::RegisterPacketEventToActor(SPtr<MessageEventInfo>&& message, UINT id)
	{
		m_server_send_message_to_actor_queue.push({ message, id });
	}

	void PacketEventManager::RegisterPacketEventToLevel(SPtr<MessageEventInfo>&& message)
	{
		m_server_send_message_to_level_queue.push(message);
	}

	void PacketEventManager::RegisterPacketEventToServer(SPtr<MessageEventInfo>&& message)
	{
		m_server_receive_message_queue.push(message);
	}

	void PacketEventManager::ConnectActorToServer(const SPtr<Actor>& actor, UINT id)
	{
		if (m_connected_actor_map[id] != nullptr || actor->IsConnectedToServer())
			LOG_WARN("This id is already registered with the server");
		else
		{
			m_connected_actor_map[id] = actor;
			actor->ConnectServer(true);
		}
	}

	SPtr<Actor> PacketEventManager::DisconnectActorFromServer(UINT id)
	{
		if (m_connected_actor_map.find(id) != m_connected_actor_map.cend()
			&& m_connected_actor_map[id] != nullptr)
		{
			SPtr<Actor> actor = m_connected_actor_map[id];
			actor->ConnectServer(false);
			m_connected_actor_map[id] = nullptr;
			return actor;
		}
		else
		{
			LOG_WARN("ID : {0} is already disconnected", id);
			return nullptr;
		}
	
	}
}
