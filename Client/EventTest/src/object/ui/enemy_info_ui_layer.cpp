#include <include/client_core.h>
#include <client/object/ui/text_ui.h>
#include "enemy_info_ui_layer.h"
#include "object/actor/rotating_cube.h"

namespace event_test
{
	EnemyInfoUILayer::EnemyInfoUILayer()
		: UserInterfaceLayer("enemy info ui layer")
	{
	}

	bool EnemyInfoUILayer::Initialize()
	{
		m_text = CreateSPtr<TextUI>("Test Text", Vec2(200.0f, 60.f), L"���ư��� ť��\n ȸ�� �ӵ� : 180");
		RegisterUserInterface(m_text);
		m_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_text->SetFontSize(20);
		
		const auto& owner = m_cube.lock();
		if (owner != nullptr)
		{
			owner->OnSpeedChangeFunction([this](float speed) {
				m_text->SetText(L"���ư��� ť��\nȸ�� �ӵ� : " + std::to_wstring(static_cast<INT>(speed)));
				});
		}

		return true;
	}

	void EnemyInfoUILayer::Update(float delta_time)
	{
	}
}