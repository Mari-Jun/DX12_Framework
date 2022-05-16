#include <include/client_core.h>
#include <client/object/ui/image_ui.h>
#include <client/object/ui/button_ui.h>
#include <client/object/ui/progress_bar_ui.h>
#include <client/object/ui/text_box_ui.h>
#include <client/object/ui/text_ui.h>
#include "event_test_ui_layer.h"

namespace event_test
{
	EventTestUILayer::EventTestUILayer()
		: UserInterfaceLayer("event test ui layer")
	{
	}

	bool EventTestUILayer::Initialize()
	{
		auto test_image = CreateSPtr<ImageUI>("Test image");
		RegisterUserInterface(test_image);
		test_image->SetTexture("../Contents/hp_background.dds");
		test_image->SetPivot(vec2::ZERO);
		test_image->SetSize(Vec2(256.f, 32.f));

		auto test_button = CreateSPtr<ButtonUI>("Test button");
		RegisterUserInterface(test_button);
		test_button->SetNormalTexture("../Contents/Penguin_Texture.dds");
		test_button->SetHoveredTexture("../Contents/Castle/SiegeRam_LOD0_Diffuse_Color.dds");
		test_button->SetPosition(Vec2(100.0f, 400.0f));
		test_button->SetSize(Vec2(400.0f, 400.0f));
		test_button->SetPivot(vec2::ZERO);

		test_button = CreateSPtr<ButtonUI>("Test button2");
		RegisterUserInterface(test_button);
		test_button->SetNormalTexture("../Contents/Penguin_Texture.dds");
		test_button->SetHoveredTexture("../Contents/Castle/SiegeRam_LOD0_Diffuse_Color.dds");
		test_button->SetPosition(Vec2(200.0f, 500.0f));
		test_button->SetPivot(vec2::ZERO);

		auto test_progress = CreateSPtr<ProgressBarUI>("Test Progress Bar");
		RegisterUserInterface(test_progress);
		test_progress->SetBackgroundTexture("../Contents/hp_background.dds");
		test_progress->SetFillTexture("../Contents/hp_bar.dds");
		test_progress->SetPosition(Vec2(200.0f, 100.0f));
		test_progress->SetSize(Vec2(256.f, 32.f));
		test_progress->SetPercent(0.5f);
		test_progress->SetPivot(vec2::ZERO);

		auto test_text = CreateSPtr<TextUI>("Test Text", Vec2(1000.f, 100.f), L"´Ù¶÷Áã Çå ÃÂ¹ÙÄû¿¡ Å¸°íÆÄ");
		RegisterUserInterface(test_text);
		test_text->SetPosition(Vec2(600.0f, 100.0f));
		test_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		test_text->SetFontSize(60);
		test_text->SetPivot(vec2::ZERO);
		//test_text_box->SetSize(Vec2(600.f, 100.f));

		auto test_text_box = CreateSPtr<TextBoxUI>("Test Text Box", Vec2(600.f, 100.f));
		RegisterUserInterface(test_text_box);
		test_text_box->SetPosition(Vec2(600.0f, 400.0f));
		test_text_box->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		test_text_box->SetPivot(vec2::ZERO);

		/*RegisterAxisEvent("test progress bar percent", { {eKey::kLArrow, -1.0f}, {eKey::kRArrow, 1.0f} },
			[test_progress](float axis)->bool {
				test_progress->SetPercent(test_progress->GetPercent() + axis * 0.01f);
				return true;
			});*/

		return true;
	}

	void EventTestUILayer::Update(float delta_time)
	{
	}
}
