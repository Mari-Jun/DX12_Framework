#include "stdafx.h"
#include "client/object/component/core/component_manager.h"
#include "client/object/component/core/component.h"

namespace client_fw
{
	void ComponentManager::Shutdown()
	{
		for (const auto& component : m_components)
			component->ShutdownComponent();
	}

	void ComponentManager::Update(float delta_time)
	{
		for (const auto& component : m_components)
			component->UpdateComponent(delta_time);
	}

	void ComponentManager::UpdateWorldMatrix()
	{
		for (const auto& component : m_components)
			component->UpdateWorldMatrix();
	}

	bool ComponentManager::RegisterComponent(const SPtr<Component>& component)
	{
		if (component->InitializeComponent())
		{
			auto iter = std::lower_bound(m_components.cbegin(), m_components.cend(), component,
				[](const SPtr<Component>& comp1, const SPtr<Component>& comp2)
				{ return comp1->GetUpdateOrder() < comp2->GetUpdateOrder(); });

			UINT index = ++m_component_names[component->GetName()];
			component->SetName(component->GetName() + std::to_string(index));

			if (iter == m_components.cend())
				m_components.push_back(component);
			else
				m_components.insert(iter, component);

			return true;
		}
		else
		{
			LOG_ERROR("Could not initialize component : {0}", component->GetName());
			component->ShutdownComponent();
			return false;
		}
	}

	void ComponentManager::UnregisterComponent(const SPtr<Component>& component)
	{
		auto [start, end] = std::equal_range(m_components.begin(), m_components.end(), component,
			[](const SPtr<Component>& comp1, const SPtr<Component>& comp2)
			{ return comp1->GetUpdateOrder() < comp2->GetUpdateOrder(); });

		auto iter = std::find(start, end, component);
		if (iter != end)
		{
			component->ShutdownComponent();
			m_components.erase(iter);
		}
	}
}
