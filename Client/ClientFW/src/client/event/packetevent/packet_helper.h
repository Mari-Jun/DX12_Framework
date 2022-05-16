#pragma once

namespace client_fw
{
	class MessageEventInfo;
	class Actor;

	/// PacketEvent�� client�� server���� ����� ���� ������ Message�� �ʿ��� �� ����� �� �ִ�.
	// ����ϱ� ���� ����� RegisterPacketEventTo[X]�� ���ؼ� message�� ����ϰ� �Ǹ�
	// Level�� Server�� ���� Message�� �ٷ� �Ѿ�� Actor�� ���� Message�� id(server�� ��ϵ�)�� ���ؼ� ���ٸ� Message�� �ް� �ȴ�.
	// 
	// Actor�� ���� ������ ������ �Ƿ��� ConnectActorToServer�Լ��� ȣ���ؼ� Actor�� ����԰� ���ÿ� server���� ������ id��
	// ���� ����ؾ��Ѵ�.
	// 
	// �� Server���� Actor�� Spawn�� �� ���� id�� �ް� �� id�� ���� connect�� �ϸ� �ȴ�.
	// 
	// �׷��� ������ message�� Actor�� Level�δ� ExecuteMessageFromServer�� ȣ���� ���ؼ� Message�� ������ �Ѿ��.
	// ���� �� Message�� ���� ó���� ExecuteMessageFromServer�Լ����� �ϸ� �ȴ�.
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

