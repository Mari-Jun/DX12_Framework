#pragma once
#include "client/object/ui/core/user_interface.h"

namespace client_fw
{
	enum class eButtonState
	{
		kUnhovered, kHovered, kPressed
	};

	class ButtonUI : public UserInterface
	{
	public:
		ButtonUI(const std::string& name = "Button", const Vec2& size = Vec2(128.f, 64.f));

		virtual bool Initialize() override;

	private:
		eButtonState m_button_state;
		SPtr<UITexture> m_normal_texture;
		SPtr<UITexture> m_hovered_texture;
		SPtr<UITexture> m_pressed_texture;

		std::function<void()> m_hovered_function;
		std::function<void()> m_pressed_function;
		std::function<void()> m_clicked_function;
		std::function<void()> m_released_function;
		std::function<void()> m_unhovered_function;

	public:
		virtual void SetPosition(const Vec2& position) override;
		virtual void SetSize(const Vec2& size) override;
		void SetNormalTexture(const std::string& path);
		void SetNormalTexture(const SPtr<Texture>& texture);
		void SetHoveredTexture(const std::string& path);
		void SetHoveredTexture(const SPtr<Texture>& texture);
		void SetPressedTexture(const std::string& path);
		void SetPressedTexture(const SPtr<Texture>& texture);

		void OnHovered(const std::function<void()>& function) { m_hovered_function = function; }
		void OnPressed(const std::function<void()>& function) { m_pressed_function = function; }
		void OnClicked(const std::function<void()>& function) { m_clicked_function = function; }
		void OnReleased(const std::function<void()>& function) { m_released_function = function; }
		void OnUnhovered(const std::function<void()>& function) { m_unhovered_function = function; }
	};
}



