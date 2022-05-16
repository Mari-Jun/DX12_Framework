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

		const SPtr<UITexture> GetBackgroundTexture() const { return m_background_texture; }
		const SPtr<UITexture> GetFillTexture() const { return m_fill_texture; }

		void SetBackgroundTexture(const std::string& path);
		void SetBackgroundTexture(const SPtr<Texture>& texture);
		void SetFillTexture(const std::string& path);
		void SetFillTexture(const SPtr<Texture>& texture);
		float GetPercent() const { return m_percent; }
		void SetPercent(float percent);

	private:
		void SetFillTextureSize(const Vec2& size);
	};
}


