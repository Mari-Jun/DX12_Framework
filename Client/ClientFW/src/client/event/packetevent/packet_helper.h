#pragma once

namespace client_fw
{
	class MessageEventInfo;
	class Actor;

	/// PacketEvent는 client와 server와의 통신을 위해 보내는 Message가 필요할 때 사용할 수 있다.
	// 사용하기 위한 방법은 RegisterPacketEventTo[X]를 통해서 message를 등록하게 되면
	// Level과 Server로 가는 Message는 바로 넘어가고 Actor로 가는 Message는 id(server에 등록된)를 통해서 같다면 Message를 받게 된다.
	// 
	// Actor의 경우는 서버와 연결이 되려면 ConnectActorToServer함수를 호출해서 Actor를 등록함과 동시에 server에서 보내준 id를
	// 같이 등록해야한다.
	// 
	// 즉 Server에서 Actor를 Spawn할 때 같이 id를 받고 그 id를 통해 connect를 하면 된다.
	// 
	// 그렇게 보내진 message는 Actor와 Level로는 ExecuteMessageFromServer의 호출을 통해서 Message의 정보가 넘어간다.
	// 따라서 각 Message에 따른 처리는 ExecuteMessageFromServer함수에서 하면 된다.
	//

	class PacketHelper
	{
	public:
		static void RegisterPacketEventToActor(SPtr<MessageEventInfo>&& message, UINT id);
		static void RegisterPacketEventToLevel(SPtr<MessageEventInfo>&& message);
		static void RegisterPacketEventToServer(SPtr<MessageEventInfo>&& message);

		static void ConnectActorToServer(const SPtr<Actor>& actor, UINT id);
		static SPtr<Actor> DisconnectActorFromServer(UINT id);
	};

}

