#include "stdafx.h"
#include "client/object/component/core/component.h"

namespace client_fw
{
	Component::Component(const std::string& name, int update_order)
		: m_name(name), m_update_order(update_order)
	{
	}

	bool Component::InitializeComponent()
	{
		UpdateWorldMatrix();
		bool ret = Initialize();
		return ret;
	}

	void Component::ShutdownComponent()
	{
		Shutdown();
	}

	void Component::UpdateComponent(float delta_time)
	{
		Update(delta_time);
	}
}
