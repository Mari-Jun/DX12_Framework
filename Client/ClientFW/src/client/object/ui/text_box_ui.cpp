#include "stdafx.h"
#include "client/object/ui/text_box_ui.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/texture/texture.h"
#include "client/event/core/event_system.h"
#include "client/input/input.h"
#include "client/event/uievent/ui_event_manager.h"
#include "client/event/uievent/ui_evevt_info.h"
#include "client/renderer/text/text_helper.h"

namespace client_fw
{
	TextBoxUI::TextBoxUI(const std::string& name, const Vec2& size)
		: TextUI(name, size, L"", Vec4(0.0f, 0.0f, 0.0f, 1.0f), 2)
	{
		m_background_texture = CreateSPtr<UITexture>(m_position, size, nullptr);
	}

	bool TextBoxUI::Initialize()
	{
		SetVisibleTexture(m_background_texture, 0);
		SetVisibleTexture(m_text_texture, 1);

		EventSystem::GetEventSystem().GetUIEventManager()->RegisterEvent(
			CreateUPtr<UIEventInfo>(shared_from_this(),
				[this]()
				{
					if (Input::IsKeyPressed(eKey::kLButton) && Input::IsConsumedKey(eKey::kLButton) == false)
					{
						Input::StartInputMethodEditor();
						m_is_modifiable = true;
						Input::OnChangeTextFromIME([this](wchar_t t) {
							std::wstring text = GetTextInfo()->GetText();
							if (t == ToUnderlying(eKey::kBackspace))
							{
								if (text.empty() == false)
									text.pop_back();
							}
							else if (t == ToUnderlying(eKey::kEnter))
							{
								Input::EndInputMethodEditor();
								Input::OnChangeTextFromIME(nullptr);
								if (m_committed_function != nullptr)
									m_committed_function();
								m_is_modifiable = false;
							}
							else
							{
								text.push_back(t);
								if (m_changed_function != nullptr)
									m_changed_function();
							}
							SetText(text);
							});
					}
				},
				[this]()
				{
					if (Input::IsKeyPressed(eKey::kLButton) && Input::IsConsumedKey(eKey::kLButton) == false)
					{
						if (m_is_modifiable == true)
						{
							if (m_committed_function != nullptr)
								m_committed_function();
							m_is_modifiable = false;
							Input::EndInputMethodEditor();
							Input::OnChangeTextFromIME(nullptr);
						}
					}
				}
		));

		return true;
	}

	void TextBoxUI::Shutdown()
	{
		Input::EndInputMethodEditor();
		Input::OnChangeTextFromIME(nullptr);
	}

	void TextBoxUI::Update(float delta_time)
	{
		TextUI::Update(delta_time);
	}

	void TextBoxUI::SetPosition(const Vec2& position)
	{
		TextUI::SetPosition(position);
		m_background_texture->SetPosition(vec2::ZERO);
	}

	void TextBoxUI::SetSize(const Vec2& size)
	{
		//추후에 Size를 변경하면 Texture 재생성
		//지금은 일단 처음 생성자에 있는 Size그대로 쓰는것을 권장함
		TextUI::SetSize(size);
		m_background_texture->SetSize(size);
	}

	void TextBoxUI::SetBackgroundTexture(const std::string& path)
	{
		const auto& texture = AssetStore::LoadTexture(path);
		if (texture != nullptr)
			SetBackgroundTexture(texture);
		else
			LOG_WARN("Could not find texture : {0}", path);
	}

	void TextBoxUI::SetBackgroundTexture(const SPtr<Texture>& texture)
	{
		m_background_texture->SetTexture(texture);
	}
}
