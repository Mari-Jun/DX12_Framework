#include <include/client_core.h>
#include <client/object/actor/sky_cube.h>

#include "object/level/rendering/basic/sky_level.h"
#include "object/levelsetting//rendering/basic/sky_level_node_manager.h"

namespace simulation
{
	UPtr<SkyCubeLevelInitNodeManager> SkyCubeLevel::s_init_node_manager = nullptr;
	UPtr<SkyCubeLevelRuntimeNodeManager> SkyCubeLevel::s_runtime_node_manager = nullptr;

	SkyCubeLevel::SkyCubeLevel()
		: SimulationLevel("sky cube level")
	{
		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<SkyCubeLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<SkyCubeLevelRuntimeNodeManager>();
	}

	bool SkyCubeLevel::Initialize()
	{
		bool ret = true;
		SpawnActor(m_sky_cube);
		return ret;
	}

	void SkyCubeLevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<SkyCubeLevel>(shared_from_this()));
	}

	void SkyCubeLevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void SkyCubeLevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<SkyCubeLevel>(shared_from_this()));
	}

	void SkyCubeLevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}
}