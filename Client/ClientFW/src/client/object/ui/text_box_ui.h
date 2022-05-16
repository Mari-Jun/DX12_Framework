#pragma once
#include "client/object/ui/text_ui.h"
#include <locale>
#include <codecvt>

namespace client_fw
{
	class TextInfo;

	class TextBoxUI : public TextUI
	{
	public:
		TextBoxUI(const std::string& name, const Vec2& size);

		virtual bool Initialize() override;
		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

	private:
		SPtr<UITexture> m_background_texture;

		bool m_is_modifiable = false;

		std::function<void()> m_changed_function;
		std::function<void()> m_committed_function;

	public:
		virtual void SetPosition(const Vec2& position) override;
		virtual void SetSize(const Vec2& size) override;
		void SetBackgroundTexture(const std::string& path);
		void SetBackgroundTexture(const SPtr<Texture>& texture);

		void OnChanged(const std::function<void()>& function) { m_changed_function = function; }
		void OnCommitted(const std::function<void()>& function) { m_committed_function = function; }
	};
}



