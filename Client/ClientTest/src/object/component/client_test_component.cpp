#include <client/core/core.h>
#include <client/core/log.h>
#include <client/math/math.h>
#include <client/input/input.h>
#include "object/component/client_test_component.h"

namespace client_test
{
	ClientTestComponent::ClientTestComponent(const std::string& name, int update_order)
		: Component(name, update_order)
	{
	}

	bool ClientTestComponent::Initialize()
	{
		LOG_INFO("Initialize {0}", m_name);
		return true;
	}

	void ClientTestComponent::Shutdown()
	{
		LOG_INFO("Shutdown {0}", m_name);
	}

	void ClientTestComponent::Update(float delta_time)
	{
	}
}