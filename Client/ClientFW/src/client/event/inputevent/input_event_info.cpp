#include "stdafx.h"
#include "client/input/input.h"
#include "client/event/inputevent//input_event_info.h"

namespace client_fw
{
	InputEventInfo::InputEventInfo(const std::string& event_name, bool is_comsume)
		: m_event_name(event_name), m_is_comsume_input(is_comsume)
	{
	}

	ActionEventInfo::ActionEventInfo(const std::string& event_name, bool is_comsume,
		std::vector<EventKeyInfo>&& event_keys, const std::function<bool()>& event_func)
		: InputEventInfo(event_name, is_comsume)
		, m_event_keys(event_keys), m_event_func(event_func)
	{
	}

	PressedEventInfo::PressedEventInfo(const std::string& event_name, bool is_comsume,
		std::vector<EventKeyInfo>&& event_keys, const std::function<bool()>& event_func)
		: ActionEventInfo(event_name, is_comsume, std::move(event_keys), event_func)
	{
	}

	void PressedEventInfo::ExecuteEvent() const
	{
		for (const auto& event_key : m_event_keys)
		{
			if (Input::IsKeyPressed(event_key.key) && Input::IsConsumedKey(event_key.key) == false &&
				std::all_of(event_key.additional_keys.cbegin(), event_key.additional_keys.cend(),
					[this](eAdditionalKey add_key)
					{
						return Input::IsKeyHoldDown(add_key);
					}))
			{
				if (m_event_func() && m_is_comsume_input)
					Input::ConsumeKey(event_key.key);
				break;
			}
		}
	}

	ReleasedEventInfo::ReleasedEventInfo(const std::string& event_name, bool is_comsume,
		std::vector<EventKeyInfo>&& event_keys, const std::function<bool()>& event_func)
		: ActionEventInfo(event_name, is_comsume, std::move(event_keys), event_func)
	{
	}

	void ReleasedEventInfo::ExecuteEvent() const
	{
		for (const auto& event_key : m_event_keys)
		{
			if (Input::IsKeyReleased(event_key.key) && Input::IsConsumedKey(event_key.key) == false &&
				std::all_of(event_key.additional_keys.cbegin(), event_key.additional_keys.cend(),
					[this](eAdditionalKey add_key)
					{
						return Input::IsKeyHoldDown(add_key);
					}))
			{
				if (m_event_func() && m_is_comsume_input)
					Input::ConsumeKey(event_key.key);
				break;
			}
		}
	}

	AxisEventInfo::AxisEventInfo(const std::string& event_name, bool is_comsume,
		std::vector<AxisEventKeyInfo>&& event_keys, const std::function<bool(float)>& event_func)
		: InputEventInfo(event_name, is_comsume)
		, m_event_keys(event_keys), m_event_func(event_func)
	{
	}

	void AxisEventInfo::ExecuteEvent() const
	{
		float axis = 0.0f;
		UINT count = 0;

		std::vector<eKey> consumptions;

		for (const auto& event_key : m_event_keys)
		{
			if (Input::IsKeyHoldDown(event_key.key) && Input::IsConsumedKey(event_key.key) == false)
			{
				axis += event_key.scale;
				++count;
				if (m_is_comsume_input)
					consumptions.push_back(event_key.key);
			}
		}

		if (axis != 0.0f && count != 0)
		{
			axis /= static_cast<float>(count);
			if (m_event_func(axis))
			{
				for(auto key : consumptions)
					Input::ConsumeKey(key);
			}
		}
	}
}
