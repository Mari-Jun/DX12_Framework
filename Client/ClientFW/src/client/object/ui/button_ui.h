#pragma once
#include "client/object/ui/core/user_interface.h"

namespace client_fw
{
	enum class eButtonState
	{
		kNormal, kHovered, kPressed
	};

	class ButtonUI : public UserInterface
	{
	public:
		ButtonUI(const std::string& name = "Button", const Vec2& size = Vec2(128.f, 64.f));

		virtual bool Initialize() override;

	private:
		eButtonState m_button_state;
		std::map<eButtonState, SPtr<UITexture>> m_button_textures;

		std::function<void()> m_hovered_function;
		std::function<void()> m_pressed_function;
		std::function<void()> m_clicked_function;
		std::function<void()> m_released_function;
		std::function<void()> m_unhovered_function;

	public:
		virtual void SetPosition(const Vec2& position) override;
		virtual void SetSize(const Vec2& size) override;

		const std::map<eButtonState, SPtr<UITexture>>& GetButtonTextures() const { return m_button_textures; }

		const Vec4& GetButtonTextureBrushColor(eButtonState button_state) const { return m_button_textures.at(button_state)->GetBrushColor(); }
		const Vec2& GetButtonTextureCoordinate(eButtonState button_state) const { return m_button_textures.at(button_state)->GetCoordinate(); }
		const Vec2& GetButtonTextureTilling(eButtonState button_state) const { return m_button_textures.at(button_state)->GetTilling(); }
		const SPtr<Texture>& GetButtonTexture(eButtonState button_state) const { return m_button_textures.at(button_state)->GetTexture(); }
		void SetButtonTextureBrushColor(eButtonState button_state, const Vec4& brush_color) const { m_button_textures.at(button_state)->SetBrushColor(brush_color); }
		void SetButtonTextureCoordinate(eButtonState button_state, const Vec2& coordinate) const { m_button_textures.at(button_state)->SetCoordinate(coordinate); }
		void SetButtonTextureTilling(eButtonState button_state, const Vec2& tilling) const { m_button_textures.at(button_state)->SetTilling(tilling); }
		void SetButtonTexture(eButtonState button_state, const SPtr<Texture>& texture) const { m_button_textures.at(button_state)->SetTexture(texture); }
		void SetNormalTexture(const std::string& path) const;
		void SetHoveredTexture(const std::string& path) const;
		void SetPressedTexture(const std::string& path) const;

		void OnHovered(const std::function<void()>& function) { m_hovered_function = function; }
		void OnPressed(const std::function<void()>& function) { m_pressed_function = function; }
		void OnClicked(const std::function<void()>& function) { m_clicked_function = function; }
		void OnReleased(const std::function<void()>& function) { m_released_function = function; }
		void OnUnhovered(const std::function<void()>& function) { m_unhovered_function = function; }
	};
}



