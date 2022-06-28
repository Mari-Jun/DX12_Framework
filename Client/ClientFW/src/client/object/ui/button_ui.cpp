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
		: UserInterface(name, size, 1), m_button_state(eButtonState::kNormal)
	{
		m_button_textures[eButtonState::kNormal] = CreateSPtr<UITexture>(m_position, m_size, nullptr);
		m_button_textures[eButtonState::kHovered] = CreateSPtr<UITexture>(m_position, m_size, nullptr);
		m_button_textures[eButtonState::kPressed] = CreateSPtr<UITexture>(m_position, m_size, nullptr);
	}

	bool ButtonUI::Initialize()
	{
		SetVisibleTexture(m_button_textures[eButtonState::kNormal], 0);

		EventSystem::GetEventSystem().GetUIEventManager()->RegisterEvent(
			CreateUPtr<UIEventInfo>(shared_from_this(),
				[this]() {
					if (m_button_state == eButtonState::kNormal)
					{
						m_button_state = eButtonState::kHovered;
						SetVisibleTexture(m_button_textures[eButtonState::kHovered], 0);
						if (m_hovered_function != nullptr)
							m_hovered_function();
					}

					if (Input::IsKeyPressed(eKey::kLButton) && Input::IsConsumedKey(eKey::kLButton) == false)
					{
						m_button_state = eButtonState::kPressed;
						SetVisibleTexture(m_button_textures[eButtonState::kPressed], 0);
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
						SetVisibleTexture(m_button_textures[eButtonState::kHovered], 0);
					}
				},
				[this]()
				{
					if (m_button_state != eButtonState::kNormal)
					{
						if ((m_button_state != eButtonState::kPressed && Input::IsKeyPressed(eKey::kLButton) == false)
							|| (m_button_state == eButtonState::kPressed && Input::IsKeyReleased(eKey::kLButton)))
						{
							m_button_state = eButtonState::kNormal;
							SetVisibleTexture(m_button_textures[eButtonState::kNormal], 0);
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
		for (const auto& [type, texture] : m_button_textures)
			texture->SetPosition(vec2::ZERO);
	}

	void ButtonUI::SetSize(const Vec2& size)
	{
		UserInterface::SetSize(size);
		for (const auto& [type, texture] : m_button_textures)
			texture->SetSize(size);
	}

	void ButtonUI::SetNormalTexture(const std::string& path) const 
	{
		const auto& texture = AssetStore::LoadTexture(path);
		if (texture != nullptr)
			SetButtonTexture(eButtonState::kNormal, texture);
		else
			LOG_WARN("Could not find texture : {0}", path);
	}

	void ButtonUI::SetHoveredTexture(const std::string& path) const
	{
		const auto& texture = AssetStore::LoadTexture(path);
		if (texture != nullptr)
			SetButtonTexture(eButtonState::kHovered, texture);
		else
			LOG_WARN("Could not find texture : {0}", path);
	}

	void ButtonUI::SetPressedTexture(const std::string& path) const
	{
		const auto& texture = AssetStore::LoadTexture(path);
		if (texture != nullptr)
			SetButtonTexture(eButtonState::kPressed, texture);
		else
			LOG_WARN("Could not find texture : {0}", path);
	}
}
