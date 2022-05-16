#include "stdafx.h"
#include "client/event/uievent/ui_event_manager.h"
#include "client/event/uievent/ui_evevt_info.h"
#include "client/object/ui/core/user_interface.h"
#include "client/input/input.h"

namespace client_fw
{
	UIEventManager::UIEventManager()
	{
	}

	UIEventManager::~UIEventManager()
	{
	}

	void UIEventManager::ExecuteEvent()
	{
		bool is_execute_hovered_event = Input::GetInputMode() == eInputMode::kGameOnly;
		UINT dead_count = 0;

		//LOG_INFO(Input::GetMousePosition());

		for (auto event_iter = m_ui_events.rbegin(); event_iter != m_ui_events.rend(); ++event_iter)
		{
			const auto& event = *event_iter;
			const auto& ui = event->GetOwnerUI();

			if (ui != nullptr)
			{
				if (ui->IsActivate())
				{
					if (is_execute_hovered_event == false && ui->IsHovered())
					{
						event->ExecuteHoveredEvent();
						is_execute_hovered_event = true;
					}
					else
						event->ExecuteUnhoveredEvent();
				}
			}
			else
			{
				std::iter_swap(event_iter, m_ui_events.rbegin() + dead_count);
				++dead_count;
			}
		}

		while (dead_count--)
			m_ui_events.pop_back();
	}

	void UIEventManager::RegisterEvent(UPtr<UIEventInfo>&& event_info)
	{
		m_ui_events.emplace_back(std::move(event_info));
	}
}
