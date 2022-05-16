#pragma once
#include "client/object/ui/core/user_interface.h"

namespace client_fw
{
	class ImageUI : public UserInterface
	{
	public:
		ImageUI(const std::string& name = "Image", const Vec2& size = Vec2(128.f, 64.f));

		virtual bool Initialize() override;

	public:
		virtual void SetPosition(const Vec2& position) override;
		virtual void SetSize(const Vec2& size) override;
		void SetTexture(const std::string& path);
		void SetTexture(const SPtr<Texture>& texture);

	protected:
		SPtr<UITexture> m_normal_texture;
	};

}

