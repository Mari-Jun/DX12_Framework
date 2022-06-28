#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/object/ui/button_ui.h>
#include <client/object/ui/text_ui.h>

#include "object/ui/ui_level_ui_layer.h"

namespace simulation
{
	UILevelUILayer::UILevelUILayer()
		: UserInterfaceLayer("ui level ui layer")
	{
		m_button = CreateSPtr<ButtonUI>("button ui", Vec2(100.0f, 100.0f));
		m_text = CreateSPtr<TextUI>("text ui", Vec2(200.0f, 25.f), L"hello world");
	}

	bool UILevelUILayer::Initialize()
	{
		bool ret = true;
		
		ret &= RegisterUserInterface(m_button);
		//m_button->SetNormalTexture("Contents/Layer/resume_level.png");
		//m_button->SetHoveredTexture("Contents/Layer/pause_level.png");
		m_button->SetPosition(Vec2(0.0f, 0.0f));
		m_button->SetPivot(Vec2(0.0f, 0.0f));
	
		ret &= RegisterUserInterface(m_text);
		m_text->SetPosition(Vec2(500.f, 500.f));
		m_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_text->SetFontName(L"배달의민족 주아");
		m_text->SetFontSize(20);

		return ret;
	}

	void UILevelUILayer::Update(float delta_time)
	{
	}
}
