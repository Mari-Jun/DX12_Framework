#pragma once

namespace client_fw
{
	class IBaseObjectManager
	{
	public:
		virtual void Shutdown() = 0;

		virtual void Update(float delta_time) = 0;
	};
}
