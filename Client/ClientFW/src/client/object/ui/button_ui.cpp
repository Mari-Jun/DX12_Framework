#include "stdafx.h"
#include "client/object/ui/button_ui.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/texture/texture.h"
#include "client/event/core/event_system.h"
#include "client/input/input.h"
#include "client/event/uievent/ui_event_manager.h"
#include "client/event/uievent/ui_evevt_info.h"

namespace client_fw
{
	ButtonUI::ButtonUI(const std::string& name, const Vec2& size)
		: UserInterface(name, size, 1), m_button_state(eButtonState::kUnhovered)
	{
		m_normal_texture = CreateSPtr<UITexture>(m_position, m_size, nullptr);
		m_hovered_texture = CreateSPtr<UITexture>(m_position, m_size, nullptr);
		m_pressed_texture = CreateSPtr<UITexture>(m_position, m_size, nullptr);
	}

	bool ButtonUI::Initialize()
	{
		SetVisibleTexture(m_normal_texture, 0);

		EventSystem::GetEventSystem().GetUIEventManager()->RegisterEvent(
			CreateUPtr<UIEventInfo>(shared_from_this(),
				[this]() {
					if (m_button_state == eButtonState::kUnhovered)
					{
						m_button_state = eButtonState::kHovered;
						SetVisibleTexture(m_hovered_texture, 0);
						if (m_hovered_function != nullptr)
							m_hovered_function();
					}

					if (Input::IsKeyPressed(eKey::kLButton) && Input::IsConsumedKey(eKey::kLButton) == false)
					{
						m_button_state = eButtonState::kPressed;
						SetVisibleTexture(m_pressed_texture, 0);
						if (m_pressed_function != nullptr)
							m_pressed_function();
					}
					if (Input::IsKeyReleased(eKey::kLButton))
					{
						if (m_released_function != nullptr)
							m_released_function();

						if (m_button_state == eButtonState::kPressed)
						{
							if (m_clicked_function != nullptr)
								m_clicked_function();
						}

						m_button_state = eButtonState::kHovered;
						SetVisibleTexture(m_hovered_texture, 0);
					}
				},
				[this]()
				{
					if (m_button_state != eButtonState::kUnhovered)
					{
						if ((m_button_state != eButtonState::kPressed && Input::IsKeyPressed(eKey::kLButton) == false)
							|| (m_button_state == eButtonState::kPressed && Input::IsKeyReleased(eKey::kLButton)))
						{
							m_button_state = eButtonState::kUnhovered;
							SetVisibleTexture(m_normal_texture, 0);
							if (m_unhovered_function != nullptr)
								m_unhovered_function();
						}
					}
				}
				));

		return true;
	}

	void ButtonUI::SetPosition(const Vec2& position)
	{
		UserInterface::SetPosition(position);
		m_normal_texture->SetPosition(vec2::ZERO);
		m_hovered_texture->SetPosition(vec2::ZERO);
		m_pressed_texture->SetPosition(vec2::ZERO);
	}

	void ButtonUI::SetSize(const Vec2& size)
	{
		UserInterface::SetSize(size);
		m_normal_texture->SetSize(size);
		m_hovered_texture->SetSize(size);
		m_pressed_texture->SetSize(size);
	}

	void ButtonUI::SetNormalTexture(const std::string& path)
	{
		const auto& texture = AssetStore::LoadTexture(path);
		if (texture != nullptr)
			SetNormalTexture(texture);
		else
			LOG_WARN("Could not find texture : {0}", path);
	}

	void ButtonUI::SetNormalTexture(const SPtr<Texture>& texture)
	{
		m_normal_texture->SetTexture(texture);
	}

	void ButtonUI::SetHoveredTexture(const std::string& path)
	{
		const auto& texture = AssetStore::LoadTexture(path);
		if (texture != nullptr)
			SetHoveredTexture(texture);
		else
			LOG_WARN("Could not find texture : {0}", path);
	}

	void ButtonUI::SetHoveredTexture(const SPtr<Texture>& texture)
	{
		m_hovered_texture->SetTexture(texture);
	}

	void ButtonUI::SetPressedTexture(const std::string& path)
	{
		const auto& texture = AssetStore::LoadTexture(path);
		if (texture != nullptr)
			SetPressedTexture(texture);
		else
			LOG_WARN("Could not find texture : {0}", path);
	}

	void ButtonUI::SetPressedTexture(const SPtr<Texture>& texture)
	{
		m_pressed_texture->SetTexture(texture);
	}
}
