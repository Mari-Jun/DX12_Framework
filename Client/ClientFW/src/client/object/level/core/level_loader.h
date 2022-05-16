#pragma once

namespace client_fw
{
	//�������̽� Ŭ������ �������� ����� �غ��� �� �� ����.
	//�ϴ� Ʋ�� ��� ���� �ɷ�.
	class Level;

	class LevelLoader
	{
	public:
		LevelLoader() = default;

		virtual void LoadLevel(const SPtr<Level>& level) = 0;
	};
}



