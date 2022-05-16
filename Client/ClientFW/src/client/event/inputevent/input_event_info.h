#pragma once
#include "client/event/core/event_info.h"

namespace client_fw
{
	struct EventKeyInfo;
	struct AxisEventKeyInfo;

	class InputEventInfo : public IEventInfo
	{
	public:
		InputEventInfo(const std::string& event_name, bool is_comsume);
		virtual ~InputEventInfo() = default;

	protected:
		std::string m_event_name;
		bool m_is_comsume_input;

	public:
		const std::string& GetEventName() const { return m_event_name; }
	};

	class ActionEventInfo : public InputEventInfo
	{
	public:
		ActionEventInfo(const std::string& event_name, bool is_comsume,
			std::vector<EventKeyInfo>&& event_keys, const std::function<bool()>& event_func);
		virtual ~ActionEventInfo() = default;

	protected:
		std::vector<EventKeyInfo> m_event_keys;
		std::function<bool()> m_event_func;
	};

	class PressedEventInfo final : public ActionEventInfo
	{
	public:
		PressedEventInfo(const std::string& event_name, bool is_comsume,
			std::vector<EventKeyInfo>&& event_keys, const std::function<bool()>& event_func);
		virtual ~PressedEventInfo() = default;

		virtual void ExecuteEvent() const override;
	};

	class ReleasedEventInfo final : public ActionEventInfo
	{
	public:
		ReleasedEventInfo(const std::string& event_name, bool is_comsume,
			std::vector<EventKeyInfo>&& event_keys, const std::function<bool()>& event_func);
		virtual ~ReleasedEventInfo() = default;

		virtual void ExecuteEvent() const override;
	};

	class AxisEventInfo final : public InputEventInfo
	{
	public:
		AxisEventInfo(const std::string& event_name, bool is_comsume,
			std::vector<AxisEventKeyInfo>&& event_keys, const std::function<bool(float)>& event_func);
		virtual ~AxisEventInfo() = default;

		virtual void ExecuteEvent() const override;

	protected:
		std::vector<AxisEventKeyInfo> m_event_keys;
		std::function<bool(float)> m_event_func;
	};
}
