#pragma once
#include "client/event/core/event_info.h"

namespace client_fw
{
	class UserInterface;

	class UIEventInfo
	{
	public:
		UIEventInfo(const WPtr<UserInterface>& owner,
			const std::function<void()>& hovered_event_func,
			const std::function<void()>& unhovered_event_func);

		virtual void ExecuteHoveredEvent() const;
		virtual void ExecuteUnhoveredEvent() const;

	protected:
		WPtr<UserInterface> m_onwer_ui;
		std::function<void()> m_hovered_event_func;
		std::function<void()> m_unhovered_event_func;

	public:
		SPtr<UserInterface> GetOwnerUI() const { return m_onwer_ui.lock(); }
	};
}


