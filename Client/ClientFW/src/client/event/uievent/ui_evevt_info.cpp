#include "stdafx.h"
#include "client/event/uievent/ui_evevt_info.h"

namespace client_fw
{
	UIEventInfo::UIEventInfo(const WPtr<UserInterface>& owner, 
		const std::function<void()>& hovered_event_func,
		const std::function<void()>& unhovered_event_func)
		: m_onwer_ui(owner)
		, m_hovered_event_func(hovered_event_func)
		, m_unhovered_event_func(unhovered_event_func)
	{
	}

	void UIEventInfo::ExecuteHoveredEvent() const
	{
		m_hovered_event_func();
	}

	void UIEventInfo::ExecuteUnhoveredEvent() const
	{
		m_unhovered_event_func();
	}
}
