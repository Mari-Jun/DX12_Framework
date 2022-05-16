#include "stdafx.h"
#include "client/object/ui/text_ui.h"
#include "client/renderer/text/text_helper.h"
#include "client/asset/texture/texture.h"

namespace client_fw
{
	TextUI::TextUI(const std::string& name, const Vec2& size,
		const std::wstring& text, size_t num_of_visible_texture)
		: UserInterface(name, size, num_of_visible_texture)
	{
		m_text_info = CreateSPtr<TextInfo>(IVec2(size), Vec4(1.f, 1.f, 1.f, 1.f),
			TextFormat(), text);
		m_text_texture = CreateSPtr<UITexture>(m_position, size, m_text_info->GetTextTexture());
	}

	TextUI::TextUI(const std::string& name, const Vec2& size,
		const std::wstring& text, const Vec4& color, size_t num_of_visible_texture)
		: UserInterface(name, size, num_of_visible_texture)
	{
		m_text_info = CreateSPtr<TextInfo>(IVec2(size), color,
			TextFormat(), text);
		m_text_texture = CreateSPtr<UITexture>(m_position, size, m_text_info->GetTextTexture());
	}

	bool TextUI::Initialize()
	{
		SetVisibleTexture(m_text_texture, 0);
		return true;
	}

	void TextUI::Update(float delta_time)
	{
		if (m_is_need_update)
		{
			m_text_info->RegisterToTextManager();
			m_is_need_update = false;
		}
	}

	void TextUI::SetPosition(const Vec2& position)
	{
		UserInterface::SetPosition(position);
		m_text_texture->SetPosition(vec2::ZERO);
	}

	void TextUI::SetSize(const Vec2& size)
	{
		UserInterface::SetSize(size);
		m_text_texture->SetSize(size);
	}

	std::string TextUI::GetText() const
	{
		return utf8_encode(m_text_info->GetText());
	}

	void TextUI::SetText(const std::string& text)
	{
		m_text_info->SetText(utf8_decode(text));
		m_is_need_update = true;
	}

	void TextUI::SetText(const std::wstring& text)
	{
		m_text_info->SetText(text);
		m_is_need_update = true;
	}

	void TextUI::SetColor(const Vec4& color)
	{
		m_text_info->SetColor(color);
		m_is_need_update = true;
	}

	void TextUI::SetFontName(const std::wstring& font_name)
	{
		m_text_info->GetTextFormat().SetFontName(font_name);
		m_is_need_update = true;
	}

	void TextUI::SetFontSize(UINT font_size)
	{
		m_text_info->GetTextFormat().SetFontSize(font_size);
		m_is_need_update = true;
	}

	void TextUI::SetFontWeight(DWRITE_FONT_WEIGHT font_weight)
	{
		m_text_info->GetTextFormat().SetFontWeight(font_weight);
		m_is_need_update = true;
	}

	void TextUI::SetFontSytle(DWRITE_FONT_STYLE font_style)
	{
		m_text_info->GetTextFormat().SetFontStyle(font_style);
		m_is_need_update = true;
	}

	void TextUI::SetTextAlignment(DWRITE_TEXT_ALIGNMENT text_alignment)
	{
		m_text_info->GetTextFormat().SetTextAlignment(text_alignment);
		m_is_need_update = true;
	}

	void TextUI::SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment)
	{
		m_text_info->GetTextFormat().SetParagraphAlignment(paragraph_alignment);
		m_is_need_update = true;
	}
}
