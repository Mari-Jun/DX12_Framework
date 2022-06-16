#pragma once
#include "client/object/component/render/billboard_component.h"

namespace client_fw
{
	class Texture;

	class TextureBillboardComponent : public BillboardComponent
	{
	public:
		TextureBillboardComponent(const std::string& name = "texture billboard");
		virtual ~TextureBillboardComponent() = default;

	protected:
		SPtr<Texture> m_texture;
		Vec2 m_offset;
		Vec2 m_tilling = Vec2(1.f, 1.f);

	public:
		const SPtr<Texture>& GetTexture() const { return m_texture; }
		void SetTexture(const SPtr<Texture>& texture) { m_texture = texture; }
		void SetTexture(const std::string& path);
		const Vec2& GetOffset() const { return m_offset; }
		void SetOffset(const Vec2& offset) { m_offset = offset; }
		const Vec2& GetTilling() const { return m_tilling; }
		void SetTilling(const Vec2& tilling) { m_tilling = tilling; }
	};
}



