#pragma once
#include "client/object/ui/core/user_interface.h"

namespace client_fw
{
	class ImageUI : public UserInterface
	{
	public:
		ImageUI(const std::string& name = "Image", const Vec2& size = Vec2(128.f, 64.f));

		virtual bool Initialize() override;

	protected:
		SPtr<UITexture> m_normal_texture;

	public:
		virtual void SetPosition(const Vec2& position) override;
		virtual void SetSize(const Vec2& size) override;

		const Vec4& GetTextureBrushColor() const { return m_normal_texture->GetBrushColor(); }
		const Vec2& GetTextureCoordinate() const { return m_normal_texture->GetCoordinate(); }
		const Vec2& GetTextureTilling() const { return m_normal_texture->GetTilling(); }
		const SPtr<Texture>& GetTexture() const { return m_normal_texture->GetTexture(); }
		void SetTextureBrushColor(const Vec4& brush_color) const { m_normal_texture->SetBrushColor(brush_color); }
		void SetTextureCoordinate(const Vec2& coordinate) const { m_normal_texture->SetCoordinate(coordinate); }
		void SetTextureTilling(const Vec2& tilling) const { m_normal_texture->SetTilling(tilling); }
		void SetTexture(const SPtr<Texture>& texture) { m_normal_texture->SetTexture(texture); }
		void SetTexture(const std::string& path);
	};

}

