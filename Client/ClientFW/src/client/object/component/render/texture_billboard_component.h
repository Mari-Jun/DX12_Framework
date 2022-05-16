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
		Vec2 m_coordinate;

	public:
		const SPtr<Texture>& GetTexture() const { return m_texture; }
		void SetTexture(const std::string& path);
		void SetTexture(const SPtr<Texture>& texture) { m_texture = texture; }
		const Vec2& GetCoordinate() const { return m_coordinate; }
		void SetCoordinate(const Vec2& coord) { m_coordinate = coord; }
	};
}



