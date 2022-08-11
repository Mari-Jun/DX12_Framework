#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/object/ui/button_ui.h>
#include <client/object/ui/text_ui.h>
#include <client/object/ui/image_ui.h>
#include <client/object/ui/progress_bar_ui.h>
#include <client/object/ui/text_box_ui.h>

#include "object/ui/ui_level_ui_layer.h"

namespace simulation
{
	UILevelUILayer::UILevelUILayer()
		: UserInterfaceLayer("ui level ui layer")
	{
		m_button = CreateSPtr<ButtonUI>("button ui", Vec2(100.0f, 100.0f));
		m_text = CreateSPtr<TextUI>("text ui", Vec2(200.0f, 25.f), L"hello world");
		m_image = CreateSPtr<ImageUI>("image ui", Vec2(200.f, 200.f));
		m_progress_bar = CreateSPtr<ProgressBarUI>("progress bar ui", Vec2(256.f, 32.f));
		m_text_box = CreateSPtr<TextBoxUI>("text ui", Vec2(200.0f, 25.f));
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

		ret &= RegisterUserInterface(m_image);
		m_image->SetPosition(Vec2(200.0f, 0.0f));
		m_image->SetPivot(Vec2(0.0f, 0.0f));

		ret &= RegisterUserInterface(m_progress_bar);
		m_progress_bar->SetPosition(Vec2(100.f, 300.f));
		m_progress_bar->SetPivot(Vec2(0.0f, 0.0f));
		m_progress_bar->SetPercent(0.5f);

		ret &= RegisterUserInterface(m_text_box);
		m_text_box->SetPosition(Vec2(500.f, 300.f));
		m_text_box->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_text_box->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_text_box->SetFontName(L"배달의민족 주아");
		m_text_box->SetFontSize(20);

		return ret;
	}

	void UILevelUILayer::Update(float delta_time)
	{
	}
}
