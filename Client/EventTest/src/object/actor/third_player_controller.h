#pragma once
#include <client/object/actor/player_controller.h>

namespace event_test
{
	using namespace client_fw;

	class ThirdPlayerController : public PlayerController
	{
	public:
		ThirdPlayerController();
		virtual ~ThirdPlayerController() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;
	};
}



