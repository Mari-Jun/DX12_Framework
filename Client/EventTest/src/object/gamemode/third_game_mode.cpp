#include <include/client_core.h>
#include "third_game_mode.h"
#include "object/actor/third_pawn.h"
#include "object/actor/third_player_controller.h"

namespace event_test
{
	ThirdGameMode::ThirdGameMode()
	{
		m_player_controller = CreateSPtr<ThirdPlayerController>();
		m_default_pawn = CreateSPtr<ThirdPawn>();
	}
}