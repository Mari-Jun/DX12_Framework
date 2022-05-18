#include "stdafx.h"
#include "client/core/window.h"
#include "client/event/core/event_system.h"
#include "client/event/inputevent/input_event_manager.h"
#include "client/event/uievent/ui_event_manager.h"
#include "client/event/messageevent/message_event_manager.h"
#include "client/event/packetevent/packet_event_manager.h"
#include "client/input/input.h"
#include "client/input/input_manager.h"

namespace client_fw
{
	EventSystem* EventSystem::s_event_system = nullptr;

	EventSystem::EventSystem(const WPtr<Window>& window)
	{
		EventSystem::s_event_system = this;
		m_input_manager = CreateUPtr<InputManager>(window);
		m_input_event_manager = CreateUPtr<InputEventManager>();
		m_ui_event_manager = CreateUPtr<UIEventManager>();
		m_message_event_manager = CreateUPtr<MessageEventManager>();
		m_packet_event_manager = CreateUPtr<PacketEventManager>();
	}

	EventSystem::~EventSystem()
	{
	}

	void EventSystem::ExecuteEvent()
	{
		m_packet_event_manager->ExecuteEventReceivedFromServer();
		eInputMode mode = Input::GetInputMode();
		if (mode != eInputMode::kInActive)
		{
			m_ui_event_manager->ExecuteEvent();
			m_input_event_manager->ExecuteEvent();
		}
		m_message_event_manager->ExecuteEvent();
		m_input_manager->Update();
	}

	void EventSystem::SendEventToServer()
	{
		m_packet_event_manager->SendEventToServer();
	}

	void EventSystem::ChangeInputState(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		static bool s_is_string_start = false;

		switch (message)
		{
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
			m_input_manager->ChangeKeyState(message, wParam, lParam);
			break;
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			m_input_manager->ChangeMouseKeyState(message, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_MOUSEMOVE:
		case WM_MOUSEHOVER:
		case WM_MOUSELEAVE:
			m_input_manager->ChangeMouseMoveState(message, wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
		case WM_IME_COMPOSITION:
			break;
		case WM_IME_KEYUP:
			m_input_manager->ChangeIMEText(wParam, lParam);
			break;
		case WM_CHAR:
			m_input_manager->ChangeIMEText(wParam, lParam);
			break;
		}
	}
}
