#pragma once
#include <concurrent_queue.h>

namespace client_fw
{
	class Actor;
	class MessageEventInfo;

	struct PacketIDEventInfo
	{
		SPtr<MessageEventInfo> message;
		UINT id;
	};
	
	// PacketEvent는 client와 server와의 통신을 위해 보내는 Message가 필요할 때 사용할 수 있다.
	// 사용하기 위한 방법은 packet_helper.h 함수를 참조하길 바란다.
	//

	class PacketEventManager
	{
	public:
		PacketEventManager();
		virtual ~PacketEventManager();

		PacketEventManager(const PacketEventManager&) = delete;
		PacketEventManager& operator=(const PacketEventManager&) = delete;

		void ExecuteEventReceivedFromServer();
		void SendEventToServer();

	public:
		void RegisterPacketEventToActor(SPtr<MessageEventInfo>&& message, UINT id);
		void RegisterPacketEventToLevel(SPtr<MessageEventInfo>&& message);
		void RegisterPacketEventToServer(SPtr<MessageEventInfo>&& message);

		void ConnectActorToServer(const SPtr<Actor>& actor, UINT id);
		SPtr<Actor> DisconnectActorFromServer(UINT id);

	private:
		Concurrency::concurrent_queue<PacketIDEventInfo> m_server_send_message_to_actor_queue;
		Concurrency::concurrent_queue<SPtr<MessageEventInfo>> m_server_send_message_to_level_queue;
		Concurrency::concurrent_queue<SPtr<MessageEventInfo>> m_server_receive_message_queue;

		std::unordered_map<UINT, SPtr<Actor>> m_connected_actor_map;
	};
}



