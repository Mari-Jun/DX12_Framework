#pragma once

namespace client_fw
{
	class PlayerController;
	class Pawn;
	class Level;

	// Level을 시작하면 사용하는 player controller와 pawn을 정의하는 클래스이다.
	class GameMode
	{
	public:
		GameMode();
		virtual ~GameMode();

		virtual bool Initialize(const SPtr<Level>& level);
		virtual void Shutdown();

	protected:
		SPtr<PlayerController> m_player_controller;
		SPtr<Pawn> m_default_pawn;

	public:
		const SPtr<PlayerController>& GetPlayerController() const { return m_player_controller; }
		const SPtr<Pawn>& GetDefaultPawn() const { return m_default_pawn; }
	};
}


