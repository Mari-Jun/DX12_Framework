#include "stdafx.h"
#include "client/object/level/gamemode/game_mode_base.h"
#include "client/object/level/core/level.h"
#include "client/object/actor/player_controller.h"
#include "client/object/actor/default_pawn.h"
#include "client/input/input.h"

namespace client_fw
{
	GameMode::GameMode()
	{
		m_player_controller = CreateSPtr<PlayerController>();
		m_default_pawn = CreateSPtr<DefaultPawn>();
	}

	GameMode::~GameMode()
	{
	}

	bool GameMode::Initialize(const SPtr<Level>& level)
	{
		m_player_controller->Possess(m_default_pawn);
		level->SpawnActor(m_default_pawn);
		level->SpawnActor(m_player_controller);
		return true;
	}

	void GameMode::Shutdown()
	{
		m_default_pawn = nullptr;
		m_player_controller = nullptr;
	}
}
