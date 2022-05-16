#include "stdafx.h"
#include "client/input/input.h"
#include "client/input/input_manager.h"
#include "client/event/inputevent/input_event_info.h"
#include "client/event/inputevent/input_event_manager.h"
#include "client/event/core/event_system.h"

namespace client_fw
{
	bool Input::IsKeyHoldDown(eKey key)
	{
		return EventSystem::GetEventSystem().GetInputManager()->IsKeyHoldDown(ToUnderlying(key));
	}

	bool Input::IsKeyHoldDown(eAdditionalKey key)
	{
		return EventSystem::GetEventSystem().GetInputManager()->IsKeyHoldDown(ToUnderlying(key));
	}

	bool Input::IsKeyPressed(eKey key)
	{
		return EventSystem::GetEventSystem().GetInputManager()->IsKeyPressed(ToUnderlying(key));
	}

	bool Input::IsKeyReleased(eKey key)
	{
		return EventSystem::GetEventSystem().GetInputManager()->IsKeyReleased(ToUnderlying(key));
	}

	bool Input::IsNotKeyHoldDown(eKey key)
	{
		return EventSystem::GetEventSystem().GetInputManager()->IsNotKeyHoldDown(ToUnderlying(key));
	}

	const IVec2& Input::GetMousePosition()
	{
		return EventSystem::GetEventSystem().GetInputManager()->GetMousePosition();
	}

	const IVec2 Input::GetRelativeMousePosition()
	{
		return EventSystem::GetEventSystem().GetInputManager()->GetRelativeMoustPosition();
	}

	void Input::ConsumeKey(eKey key)
	{
		EventSystem::GetEventSystem().GetInputManager()->ConsumeKey(ToUnderlying(key));
	}

	bool Input::IsConsumedKey(eKey key)
	{
		return EventSystem::GetEventSystem().GetInputManager()->IsConsumedKey(ToUnderlying(key));
	}

	void Input::SetHideCursor(bool hide)
	{
		if (hide != IsHideCursor())
			EventSystem::GetEventSystem().GetInputManager()->SetHideCursor(hide);
	}

	bool Input::IsHideCursor()
	{
		return EventSystem::GetEventSystem().GetInputManager()->IsHideCursor();
	}

	void Input::SetClipCursor(bool clip)
	{
		if (clip != IsClipCursor())
			EventSystem::GetEventSystem().GetInputManager()->SetClipCursor(clip);
	}

	bool Input::IsClipCursor()
	{
		return EventSystem::GetEventSystem().GetInputManager()->IsClipCursor();
	}

	void Input::StartInputMethodEditor()
	{
		EventSystem::GetEventSystem().GetInputManager()->StartInputMethodEditor();
	}

	void Input::EndInputMethodEditor()
	{
		EventSystem::GetEventSystem().GetInputManager()->EndInputMethodEditor();
	}

	void Input::OnChangeTextFromIME(const std::function<void(wchar_t)>& function)
	{
		EventSystem::GetEventSystem().GetInputManager()->OnChangeTextFromIME(function);
	}

	bool Input::RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption, eInputOwnerType type)
	{
		return EventSystem::GetEventSystem().GetInputEventManager()->RegisterEvent(
			CreateUPtr<PressedEventInfo>(name, consumption, std::move(keys), func), type
		);
	}

	bool Input::RegisterReleasedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption, eInputOwnerType type)
	{
		return EventSystem::GetEventSystem().GetInputEventManager()->RegisterEvent(
			CreateUPtr<ReleasedEventInfo>(name, consumption, std::move(keys), func), type
		);
	}

	bool Input::RegisterAxisEvent(const std::string& name, std::vector<AxisEventKeyInfo>&& keys,
		const std::function<bool(float)>& func, bool consumption, eInputOwnerType type)
	{
		return EventSystem::GetEventSystem().GetInputEventManager()->RegisterEvent(
			CreateUPtr<AxisEventInfo>(name, consumption, std::move(keys), func), type
		);
	}

	void Input::UnregisterInputEvent(const std::string& name)
	{
		EventSystem::GetEventSystem().GetInputEventManager()->UnregisterEvent(name);
	}

	eInputMode Input::GetInputMode()
	{
		return EventSystem::GetEventSystem().GetInputEventManager()->GetInputMode();
	}

	void Input::SetInputMode(eInputMode input_mode)
	{
		switch (input_mode)
		{
		case client_fw::eInputMode::kUIOnly:
			Input::SetHideCursor(false);
			Input::SetClipCursor(true);
			break;
		case client_fw::eInputMode::kUIAndGame:
			Input::SetHideCursor(false);
			Input::SetClipCursor(true);
			break;
		case client_fw::eInputMode::kGameOnly:
			Input::SetHideCursor(true);
			Input::SetClipCursor(true);
			break;
		case client_fw::eInputMode::kInActive:
			Input::SetHideCursor(false);
			Input::SetClipCursor(false);
			break;
		default:
			break;
		}
		EventSystem::GetEventSystem().GetInputEventManager()->SetInputMode(input_mode);
	}

	void Input::SetOnlyInputMode(eInputMode input_mode)
	{
		EventSystem::GetEventSystem().GetInputEventManager()->SetInputMode(input_mode);
	}
}