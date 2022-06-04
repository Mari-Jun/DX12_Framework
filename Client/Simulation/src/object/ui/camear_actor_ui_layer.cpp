#include <include/client_core.h>
#include <client/object/ui/text_ui.h>
#include <client/util/file_dialog.h>

#include "object/actor/camera_actor.h"
#include "object/ui/camear_actor_ui_layer.h"

namespace simulation
{
	CameraActorUILayer::CameraActorUILayer()
		: UserInterfaceLayer("camera actor ui layer")
	{
	}

	bool CameraActorUILayer::Initialize()
	{
		const auto& owner = m_owner.lock();

		m_text = CreateSPtr<TextUI>("owner name", Vec2(200.0f, 25.f), utf8_decode(owner->GetName()));
		RegisterUserInterface(m_text);
		m_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_text->SetFontName(L"배달의민족 주아");
		m_text->SetFontSize(20);

		return true;
	}

	void CameraActorUILayer::Update(float delta_time)
	{
	}

	CameraPawnUILayer::CameraPawnUILayer()
		: UserInterfaceLayer("camera pawn ui layer")
	{
	}

	bool CameraPawnUILayer::Initialize()
	{
		const auto& owner = m_owner.lock();

		m_text = CreateSPtr<TextUI>("owner name", Vec2(200.0f, 25.f), utf8_decode(owner->GetName()));
		RegisterUserInterface(m_text);
		m_text->SetFontWeight(DWRITE_FONT_WEIGHT_BOLD);
		m_text->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_text->SetFontName(L"배달의민족 주아");
		m_text->SetFontSize(20);

		return true;
	}

	void CameraPawnUILayer::Update(float delta_time)
	{
	}
}