#include "stdafx.h"
#include "client/renderer/text/text_helper.h"
#include "client/renderer/text/text_render_system.h"
#include "client/renderer/text/text_render_manager.h"
#include "client/asset/texture/texture.h"

namespace client_fw
{
	BrushColor::BrushColor(const Vec4& color)
		: m_color(color)
	{
	}

	bool operator<(const BrushColor& b1, const BrushColor& b2)
	{
		if (b1.GetColor().x == b2.GetColor().x)
		{
			if (b1.GetColor().y == b2.GetColor().y)
			{
				if (b1.GetColor().z == b2.GetColor().z)
					return b1.GetColor().w < b2.GetColor().w;
				else
					return b1.GetColor().z < b2.GetColor().z;
			}
			return b1.GetColor().y < b2.GetColor().y;
		}
		return b1.GetColor().x < b2.GetColor().x;
	}

	TextFormat::TextFormat(const std::wstring& name, UINT size)
		: m_font_name(name), m_font_size(size)
	{
	}

	bool operator<(const TextFormat& t1, const TextFormat& t2)
	{
		if (t1.GetFontName() == t2.GetFontName())
		{
			if (t1.GetFontSize() == t2.GetFontSize())
			{
				if (t1.GetFontWeight() == t2.GetFontWeight())
				{
					if (t1.GetFontStyle() == t2.GetFontStyle())
					{
						if (t1.GetTextAlignment() == t2.GetTextAlignment())
							return t1.GetParagraphAlignment() < t2.GetParagraphAlignment();
						else
							return t1.GetTextAlignment() < t2.GetTextAlignment();
					}
					else return t1.GetFontStyle() < t2.GetFontStyle();
				}
				else return t1.GetFontWeight() < t2.GetFontWeight();
			}
			else return t1.GetFontSize() < t2.GetFontSize();
		}
		else return t1.GetFontName() < t2.GetFontName();
	}

	TextInfo::TextInfo(const IVec2& size, const Vec4& color, 
		const TextFormat& format, const std::wstring& text)
		: m_text(text), m_color(BrushColor(color))
		, m_format(format)
	{
		m_text_texture = CreateSPtr<RenderTextTexture>(size);
	}

	TextInfo::~TextInfo()
	{
		m_text_texture->Shutdown();
	}

	void TextInfo::RegisterToTextManager()
	{
		TextHelper::RegisterTextToTextManager(shared_from_this());
	}

	void TextHelper::RegisterTextToTextManager(const SPtr<TextInfo>& info)
	{
#ifdef __USE_DWRITE__
		s_render_system->GetTextRenderManager()->RegisterText(info);
#endif
	}
}
