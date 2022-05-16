#pragma once

namespace client_fw
{
	//인터페이스 클래스로 만들지는 고민을 해봐야 할 것 같다.
	//일단 틀만 잡아 놓는 걸로.
	class Level;

	class LevelLoader
	{
	public:
		LevelLoader() = default;

		virtual void LoadLevel(const SPtr<Level>& level) = 0;
	};
}



