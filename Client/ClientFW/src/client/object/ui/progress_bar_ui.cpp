#include "stdafx.h"
#include "client/object/ui/progress_bar_ui.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/texture/texture.h"

namespace client_fw
{
	ProgressBarUI::ProgressBarUI(const std::string& name, const Vec2& size)
		: UserInterface(name, size, 2)
	{
		m_background_texture = CreateSPtr<UITexture>(m_position, m_size, nullptr);
		m_fill_texture = CreateSPtr<UITexture>(m_position, m_size, nullptr);
	}

	bool ProgressBarUI::Initialize()
	{
		SetVisibleTexture(m_background_texture, 0);
		SetVisibleTexture(m_fill_texture, 1);

		return true;
	}

	void ProgressBarUI::SetPosition(const Vec2& position)
	{
		UserInterface::SetPosition(position);
		m_background_texture->SetPosition(vec2::ZERO);
		m_fill_texture->SetPosition(vec2::ZERO);
	}

	void ProgressBarUI::SetSize(const Vec2& size)
	{
		UserInterface::SetSize(size);
		m_background_texture->SetSize(size);
		SetFillTextureSize(size);
	}

	void ProgressBarUI::SetBackgroundTexture(const std::string& path)
	{
		const auto& texture = AssetStore::LoadTexture(path);
		if (texture != nullptr)
			SetBackgroundTexture(texture);
		else
			LOG_WARN("Could not find texture : {0}", path);
	}

	void ProgressBarUI::SetFillTexture(const std::string& path)
	{
		const auto& texture = AssetStore::LoadTexture(path);
		if (texture != nullptr)
			SetFillTexture(texture);
		else
			LOG_WARN("Could not find texture : {0}", path);
	}

	void ProgressBarUI::SetPercent(float percent)
	{
		m_percent = std::clamp(percent, 0.0f, 1.0f);
		SetFillTextureSize(m_size);
	}

	void ProgressBarUI::SetFillTextureSize(const Vec2& size)
	{
		m_fill_texture->SetSize(Vec2(size.x * m_percent, size.y));
		m_fill_texture->SetTilling(Vec2(m_percent, 1.0f));
	}
}
