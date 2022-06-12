#include <include/client_core.h>

#include "object/actor/camera_actor.h"
#include "object/gamemode/camera_game_mode.h"

namespace simulation
{
	CameraGameMode::CameraGameMode(bool use_controller_event)
	{
		m_player_controller = CreateSPtr<CameraPlayerController>(use_controller_event);
		m_default_pawn = CreateSPtr<CameraPawn>();
	}
}