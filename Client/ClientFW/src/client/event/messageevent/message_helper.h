#pragma once

namespace client_fw
{
	class Actor;
	class Level;
	class MessageEventInfo;

	// MessageEvent�� client������ �ٸ� Actor�� Level���� ������ Message�� �ʿ��� �� ����� �� �ִ�.
	// ����ϱ� ���� ����� RegisterMessageEvent�� ���ؼ� message�� ����ϰ� �Ǹ�
	// �� message�� registerMessageReceiver�� ���� ��ϵ� id�� ���ؼ� ������ ���� ��ϵ� actor���� ��������
	// level�� ���� ���� ����� �ʿ� ���� ���� �����ִ� level�� message�� �������� �ȴ�.
	// �� message�� �����°����� RegisterMessageEvent�� ȣ���ϰ�
	// �޴°����� RegisterMessageReceiver�� ȣ���ϰ� �Ǹ� Message�� Actor�Ǵ� Level�� ExecuteMessage�Լ��� ȣ���ϸ鼭
	// ������ �Ѿ�� �ȴ�.
	// ���� Actor, Level�� ExecuteMessage �Լ��� ���� ������ �����ϸ� �ȴ�.
	//

	class MessageHelper final
	{
	public:
		static void RegisterMessageEvent(SPtr<MessageEventInfo>&& message);

		static void RegisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor);
		static void UnregisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor);
	};
}



