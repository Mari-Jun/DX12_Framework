#pragma once
#include "client/object/ui/core/user_interface.h"
#include "client/renderer/text/text_helper.h"

namespace client_fw
{
    class TextUI : public UserInterface
    {
    public:
        TextUI(const std::string& name, const Vec2& size,
            const std::wstring& text, size_t num_of_visible_texture = 1);
        TextUI(const std::string& name, const Vec2& size,
            const std::wstring& text, const Vec4& color, size_t num_of_visible_texture = 1);

        virtual bool Initialize() override;
        virtual void Update(float delta_time) override;

    protected:
        SPtr<UITexture> m_text_texture;
        SPtr<TextInfo> m_text_info;
        bool m_is_need_update = true;

    public:
        virtual void SetPosition(const Vec2& position) override;
        virtual void SetSize(const Vec2& size) override;

        const SPtr<TextInfo>& GetTextInfo() const { return m_text_info; }
        std::string GetText() const;
        void SetText(const std::string& text);
        void SetText(const std::wstring& text);
        void SetColor(const Vec4& color);
        void SetFontName(const std::wstring& font_name);
        void SetFontSize(UINT font_size);
        void SetFontWeight(DWRITE_FONT_WEIGHT font_weight);
        void SetFontSytle(DWRITE_FONT_STYLE font_style);
        void SetTextAlignment(DWRITE_TEXT_ALIGNMENT text_alignment);
        void SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT paragraph_alignment);
    };
}


