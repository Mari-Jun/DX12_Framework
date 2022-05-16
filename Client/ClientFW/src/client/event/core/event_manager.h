#pragma once

namespace client_fw
{
	class IEventManager
	{
	public:
		IEventManager() {}
		virtual ~IEventManager() = default;

		IEventManager(const IEventManager&) = delete;
		IEventManager& operator=(const IEventManager&) = delete;

		virtual void ExecuteEvent() = 0;
	};
}

