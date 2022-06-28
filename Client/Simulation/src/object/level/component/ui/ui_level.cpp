#include <include/client_core.h>

#include "object/level/component/ui/ui_level.h"
#include "object/levelsetting/component/ui/ui_level_node_manager.h"
#include "object/ui/ui_level_ui_layer.h"

namespace simulation
{
	UPtr<UILevelInitNodeManager> UILevel::s_init_node_manager = nullptr;
	UPtr<UILevelRuntimeNodeManager> UILevel::s_runtime_node_manager = nullptr;

	UILevel::UILevel()
		: SimulationLevel("ui level")
	{
		m_ui_layer = CreateSPtr<UILevelUILayer>();

		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<UILevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<UILevelRuntimeNodeManager>();
	}

	bool UILevel::Initialize()
	{
		bool ret = SimulationLevel::Initialize();

		RegisterUILayer(m_ui_layer);

		return ret;
	}

	void UILevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<UILevel>(shared_from_this()));
	}

	void UILevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void UILevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<UILevel>(shared_from_this()));
	}

	void UILevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}
}
