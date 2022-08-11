#pragma once
#include "client/object/ui/core/user_interface.h"

namespace client_fw
{
	class ProgressBarUI : public UserInterface
	{
	public:
		ProgressBarUI(const std::string& name = "Progress bar", const Vec2& size = Vec2(256.f, 32.f));

		virtual bool Initialize() override;

	private:
		SPtr<UITexture> m_background_texture;
		SPtr<UITexture> m_fill_texture;

		float m_percent = 0.0f;

	public:
		virtual void SetPosition(const Vec2& position) override;
		virtual void SetSize(const Vec2& size) override;

		const Vec4& GetBackgroundTextureBrushColor() const { return m_background_texture->GetBrushColor(); }
		const Vec2& GetBackgroundTextureCoordinate() const { return m_background_texture->GetCoordinate(); }
		const Vec2& GetBackgroundTextureTilling() const { return m_background_texture->GetTilling(); }
		const SPtr<Texture> GetBackgroundTexture() const { return m_background_texture->GetTexture(); }
		void SetBackgroundTextureBrushColor(const Vec4& brush_color) const { m_background_texture->SetBrushColor(brush_color); }
		void SetBackgroundTextureCoordinate(const Vec2& coordinate) const { m_background_texture->SetCoordinate(coordinate); }
		void SetBackgroundTextureTilling(const Vec2& tilling) const { m_background_texture->SetTilling(tilling); }
		void SetBackgroundTexture(const SPtr<Texture>& texture) { m_background_texture->SetTexture(texture); }
		void SetBackgroundTexture(const std::string& path);

		const Vec4& GetFillTextureBrushColor() const { return m_fill_texture->GetBrushColor(); }
		const Vec2& GetFillTextureCoordinate() const { return m_fill_texture->GetCoordinate(); }
		const Vec2& GetFillTextureTilling() const { return m_fill_texture->GetTilling(); }
		const SPtr<Texture> GetFillTexture() const { return m_fill_texture->GetTexture(); }
		void SetFillTextureBrushColor(const Vec4& brush_color) const { m_fill_texture->SetBrushColor(brush_color); }
		void SetFillTextureCoordinate(const Vec2& coordinate) const { m_fill_texture->SetCoordinate(coordinate); }
		void SetFillTextureTilling(const Vec2& tilling) const { m_fill_texture->SetTilling(tilling); }
		void SetFillTexture(const SPtr<Texture>& texture) { m_fill_texture->SetTexture(texture); }
		void SetFillTexture(const std::string& path);

		float GetPercent() const { return m_percent; }
		void SetPercent(float percent);

	private:
		void SetFillTextureSize(const Vec2& size);
	};
}


