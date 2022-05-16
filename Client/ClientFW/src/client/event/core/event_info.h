#pragma once

namespace client_fw
{
	class IEventInfo
	{
	public:
		IEventInfo() {}
		virtual ~IEventInfo() = default;

		virtual void ExecuteEvent() const = 0;
	};
}



