#pragma once
#include "client/object/core/base_object_manager.h"

namespace client_fw
{
	class Component;

	class ComponentManager final : public IBaseObjectManager
	{
	public:
		ComponentManager() = default;
		virtual ~ComponentManager() = default;

		ComponentManager(const ComponentManager&) = delete;
		ComponentManager& operator=(const ComponentManager&) = delete;

		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;
		void UpdateWorldMatrix();

		bool RegisterComponent(const SPtr<Component>& component);
		void UnregisterComponent(const SPtr<Component>& component);

	private:
		std::vector<SPtr<Component>> m_components;

		std::map<std::string, UINT> m_component_names;
	};

}

