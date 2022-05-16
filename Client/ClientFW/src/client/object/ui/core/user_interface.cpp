#include "stdafx.h"
#include "client/object/ui/core/user_interface.h"
#include "client/renderer/core/render.h"
#include "client/input/input.h"

namespace client_fw
{
	UserInterface::UserInterface(const std::string& name, const Vec2& size, size_t num_of_visible_texture)
		: m_name(name), m_position(vec2::ZERO), m_size(size)
	{
		m_visible_textures.resize(num_of_visible_texture);
	}

	UserInterface::~UserInterface()
	{
	}

	void UserInterface::SetPosition(const Vec2& position)
	{
		m_position = position;
		m_pivot_position = m_position - m_pivot * m_size;
	}

	void UserInterface::SetSize(const Vec2& size)
	{
		m_size = size;
		SetPosition(GetPosition());
	}

	void UserInterface::SetPivot(const Vec2& pivot)
	{
		m_pivot.x = std::clamp(pivot.x, 0.0f, 1.0f);
		m_pivot.y = std::clamp(pivot.y, 0.0f, 1.0f);
		SetPosition(GetPosition());
	}

	bool UserInterface::IsHovered() const
	{
		Vec2 pos = Vec2(static_cast<float>(Input::GetMousePosition().x),
			static_cast<float>(Input::GetMousePosition().y));
		return (pos >= m_pivot_position && pos <= m_pivot_position + m_size);
	}

	void UserInterface::SetVisibleTexture(const SPtr<UITexture>& texture, UINT index)
	{
		if (index < m_visible_textures.size())
		{
			m_visible_textures[index] = texture;
		}
	}
}
