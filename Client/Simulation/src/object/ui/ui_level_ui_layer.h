#pragma once
#include <client/object/ui/core/user_interface_layer.h>

namespace client_fw
{
	class ButtonUI;
	class ImageUI;
	class ProgressBarUI;
	class TextUI;
	class TextBoxUI;
}

namespace simulation
{
	using namespace client_fw;

	class UILevelUILayer : public UserInterfaceLayer
	{
	public:
		UILevelUILayer();
		virtual ~UILevelUILayer() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;
		
	private:
		SPtr<ButtonUI> m_button;
		SPtr<TextUI> m_text;
		SPtr<ImageUI> m_image;
		SPtr<ProgressBarUI> m_progress_bar;
		SPtr<TextBoxUI> m_text_box;

	public:
		const SPtr<ButtonUI>& GetButton() const { return m_button; }
		const SPtr<TextUI>& GetText() const { return m_text; }
		const SPtr<ImageUI>& GetImage() const { return m_image; }
		const SPtr<ProgressBarUI>& GetProgressBar() const { return m_progress_bar; }
		const SPtr<TextBoxUI>& GetTextBox() const { return m_text_box; }
	};
}



