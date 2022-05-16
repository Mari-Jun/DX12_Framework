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

    //utf8_encode and decode code is copy and paste from stack overflow

    static std::string utf8_encode(const std::wstring& wstr)
    {
        if (wstr.empty()) return std::string();
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
        std::string str_to(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str_to[0], size_needed, NULL, NULL);
        return str_to;
    }

    static std::wstring utf8_decode(const std::string& str)
    {
        if (str.empty()) return std::wstring();
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
        std::wstring wstr_to(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr_to[0], size_needed);
        return wstr_to;
    }
}


