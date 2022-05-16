#pragma once

namespace client_fw
{
	class Actor;
	class Level;
	class MessageEventInfo;

	// MessageEvent는 client내에서 다른 Actor나 Level에게 보내는 Message가 필요할 때 사용할 수 있다.
	// 사용하기 위한 방법은 RegisterMessageEvent를 통해서 message를 등록하게 되면
	// 그 message가 registerMessageReceiver를 통해 등록된 id와 비교해서 같으면 같이 등록된 actor에게 보내지고
	// level의 경우는 따로 등록할 필요 없이 현재 열려있는 level에 message가 보내지게 된다.
	// 즉 message를 보내는곳에는 RegisterMessageEvent를 호출하고
	// 받는곳에는 RegisterMessageReceiver를 호출하게 되면 Message가 Actor또는 Level에 ExecuteMessage함수를 호출하면서
	// 정보가 넘어가게 된다.
	// 따라서 Actor, Level에 ExecuteMessage 함수에 세부 내용을 정의하면 된다.
	//

	class MessageHelper final
	{
	public:
		static void RegisterMessageEvent(SPtr<MessageEventInfo>&& message);

		static void RegisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor);
		static void UnregisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor);
	};
}



