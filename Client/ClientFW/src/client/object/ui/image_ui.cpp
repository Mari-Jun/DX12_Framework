#include "stdafx.h"
#include "client/object/ui/image_ui.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/texture/texture.h"

namespace client_fw
{
	ImageUI::ImageUI(const std::string& name, const Vec2& size)
		: UserInterface(name, size, 1)
	{
		m_normal_texture = CreateSPtr<UITexture>(m_position, m_size, nullptr);
	}

	bool ImageUI::Initialize()
	{
		SetVisibleTexture(m_normal_texture, 0);
		return true;
	}

	void ImageUI::SetPosition(const Vec2& position)
	{
		UserInterface::SetPosition(position);
		m_normal_texture->SetPosition(vec2::ZERO);
	}

	void ImageUI::SetSize(const Vec2& size)
	{
		UserInterface::SetSize(size);
		m_normal_texture->SetSize(size);
	}

	void ImageUI::SetTexture(const std::string& path)
	{
		const auto& texture = AssetStore::LoadTexture(path);
		if (texture != nullptr)
			SetTexture(texture);
		else
			LOG_WARN("Could not find texture : {0}", path);
	}
}
