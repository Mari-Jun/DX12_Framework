#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/light.h>

#include "object/level/rotating_cube_level.h"

#include "object/levelsetting/rotating_cube_level_node_manager.h"

#include "object/actor/rotating_cube.h"

namespace simulation
{
	UPtr<RotatingCubeLevelInitNodeManager> RotatingCubeLevel::m_init_node_manager = nullptr;
	UPtr<RotatingCubeLevelRuntimeNodeManager> RotatingCubeLevel::m_runtime_node_manager = nullptr;

	RotatingCubeLevel::RotatingCubeLevel()
		: SimulationLevel("rotating cube level")
	{
		m_rotating_cube = CreateSPtr<RotatingCube>();
		if (m_init_node_manager == nullptr)
			m_init_node_manager = CreateUPtr<RotatingCubeLevelInitNodeManager>();
		if (m_runtime_node_manager == nullptr)
			m_runtime_node_manager = CreateUPtr<RotatingCubeLevelRuntimeNodeManager>();
	}

	bool RotatingCubeLevel::Initialize()
	{
		SpawnActor(m_rotating_cube);
		m_rotating_cube->SetPosition(m_init_pos);
		m_rotating_cube->SetScale(m_init_scale);

		m_directional_light = CreateSPtr<DirectionalLight>();
		m_directional_light->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		m_directional_light->SetRotation(math::ToRadian(45.0f), 0.0f, 0.0f);
		if(m_enable_dir_light_shadow == false)
			m_directional_light->DisableShadow();
		SpawnActor(m_directional_light);

		return true;
	}

	void RotatingCubeLevel::Shutdown()
	{
	}

	void RotatingCubeLevel::Update(float delta_time)
	{
	}

	void RotatingCubeLevel::SetLevelInitNodeOwner()
	{
		m_init_node_manager->SetOwner(std::static_pointer_cast<RotatingCubeLevel>(shared_from_this()));
	}

	void RotatingCubeLevel::ExecuteLevelInitNodes()
	{
		m_init_node_manager->ExecuteLevelSettingNodes();
	}

	void RotatingCubeLevel::SetLevelRuntimeNodeOwner()
	{
		m_runtime_node_manager->SetOwner(std::static_pointer_cast<RotatingCubeLevel>(shared_from_this()));
	}

	void RotatingCubeLevel::ExecuteLevelRuntimeNodes()
	{
		m_runtime_node_manager->ExecuteLevelSettingNodes();
	}
}
