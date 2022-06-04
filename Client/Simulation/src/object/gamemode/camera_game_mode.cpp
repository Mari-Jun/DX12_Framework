#include <include/client_core.h>

#include "object/actor/camera_actor.h"
#include "object/gamemode/camera_game_mode.h"

namespace simulation
{
	CameraGameMode::CameraGameMode()
	{
		m_player_controller = CreateSPtr<CameraPlayerController>();
		m_default_pawn = CreateSPtr<CameraPawn>();
	}
}