#pragma once
#include <dwrite.h>

namespace client_fw
{
	class TextRenderSystem;
	class RenderTextTexture;

	class BrushColor
	{
	public:
		BrushColor(const Vec4& color);

	private:
		Vec4 m_color;
		
	public:
		const Vec4& GetColor() const { return m_color; }
		void SetColor(const Vec4& color) { m_color = color; }
	};

	bool operator<(const BrushColor& b1, const BrushColor& b2);

	class TextFormat
	{
	public:
		TextFormat(const std::wstring& name = L"Segoe UI", UINT size = 24);

	private:
		std::wstring m_font_name;
		UINT m_font_size = 24;
		DWRITE_FONT_WEIGHT m_font_weight = DWRITE_FONT_WEIGHT_NORMAL;
		DWRITE_FONT_STYLE m_font_style = DWRITE_FONT_STYLE_NORMAL;
		DWRITE_TEXT_ALIGNMENT m_text_alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
		DWRITE_PARAGRAPH_ALIGNMENT m_paragraph_alignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;

	public:
		const std::wstring& GetFontName() const { return m_font_name; }
		void SetFontName(const std::wstring& font_name) { m_font_name = font_name; }
		UINT GetFontSize() const { return m_font_size; }
		void SetFontSize(UINT font_size) { m_font_size = font_size; }
		DWRITE_FONT_WEIGHT GetFontWeight() const { return m_font_weight; }
		void SetFontWeight(DWRITE_FONT_WEIGHT font_weight) { m_font_weight = font_weight; }
		DWRITE_FONT_STYLE GetFontStyle() const { return m_font_style; }
		void SetFontStyle(DWRITE_FONT_STYLE font_style) { m_font_style = font_style; }
		DWRITE_TEXT_ALIGNMENT GetTextAlignment() const { return m_text_alignment; }
		void SetTextAlignment(DWRITE_TEXT_ALIGNMENT text_alignment) { m_text_alignment = text_alignment; }
		DWRITE_PARAGRAPH_ALIGNMENT GetParagraphAlignment() const { return m_paragraph_alignment; }
		void SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment) { m_paragraph_alignment = paragraph_alignment; }
	};

	bool operator<(const TextFormat& t1, const TextFormat& t2);

	class TextInfo : public std::enable_shared_from_this<TextInfo>
	{
	public:
		TextInfo(const IVec2& size, const Vec4& color, const TextFormat& format, const std::wstring& text = L"");

		void Shutdown();
		void RegisterToTextManager();

	private:
		std::wstring m_text;
		BrushColor m_color;
		TextFormat m_format;
		SPtr<RenderTextTexture> m_text_texture;

	public:
		const std::wstring& GetText() const { return m_text; }
		void SetText(const std::wstring& text) { m_text = text; }
		const BrushColor& GetColor() const { return m_color; }
		void SetColor(const Vec4& color) { m_color = BrushColor(color); }
		const TextFormat& GetTextFormat() const { return m_format; }
		TextFormat& GetTextFormat() { return m_format; }


		const SPtr<RenderTextTexture>& GetTextTexture() const { return m_text_texture; }
	};


	class TextHelper
	{
	public:
		static void RegisterTextToTextManager(const SPtr<TextInfo>& info);

	private:
		friend TextRenderSystem;
		static TextRenderSystem* s_render_system;
	};

}

