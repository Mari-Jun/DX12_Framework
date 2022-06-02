#include <include/client_core.h>
#include <client/object/actor/light.h>

#include "object/actor/camera_actor.h"
#include "object/level/multi_camera_level.h"
#include "object/levelsetting/multi_camera_level_node_manager.h"

namespace simulation
{
	UPtr<MultiCameraLevelInitNodeManager> MultiCameraLevel::s_init_node_manager = nullptr;
	UPtr<MultiCameraLevelRuntimeNodeManager> MultiCameraLevel::s_runtime_node_manager = nullptr;

	MultiCameraLevel::MultiCameraLevel()
		: SimulationLevel("multi camera level")
	{
		m_directional_light = CreateSPtr<DirectionalLight>();
		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<MultiCameraLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<MultiCameraLevelRuntimeNodeManager>();
	}

	bool MultiCameraLevel::Initialize()
	{
		bool ret = SimulationLevel::Initialize();

		m_directional_light->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		m_directional_light->SetRotation(math::ToRadian(45.0f), 0.0f, 0.0f);
		m_directional_light->DisableShadow();
		SpawnActor(m_directional_light);

		UINT num_of_camera = s_init_node_manager->GetNumOfCamera();
		float offset = s_init_node_manager->GetCameraOffset();
		for (UINT i = 0; i < num_of_camera; ++i)
		{
			float radian = i * (XM_2PI / num_of_camera);

			auto camera_actor = CreateSPtr<CameraActor>();
			camera_actor->SetPosition(Vec3(sin(radian) * offset, 0.0f, cos(radian) * offset));
			camera_actor->SetRotation(0.0f, radian + XM_PI, 0.0f);
			SpawnActor(camera_actor);

			m_camera_actors.push_back(camera_actor);
		}

		return ret;
	}

	void MultiCameraLevel::Shutdown()
	{
	}

	void MultiCameraLevel::Update(float delta_time)
	{
	}

	void MultiCameraLevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<MultiCameraLevel>(shared_from_this()));
	}

	void MultiCameraLevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void MultiCameraLevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<MultiCameraLevel>(shared_from_this()));
	}

	void MultiCameraLevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}
}
