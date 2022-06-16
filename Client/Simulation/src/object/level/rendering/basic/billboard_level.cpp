#include <include/client_core.h>
#include <client/object/actor/light.h>

#include "object/actor/billboard_actor.h"

#include "object/level/rendering/basic/billboard_level.h"
#include "object/levelsetting/rendering/basic/billboard_level_node_manager.h"

namespace simulation
{
	UPtr<BillboardLevelInitNodeManager> TextureBillboardLevel::s_init_node_manager = nullptr;
	UPtr<BillboardLevelRuntimeNodeManager> TextureBillboardLevel::s_runtime_node_manager = nullptr;

	TextureBillboardLevel::TextureBillboardLevel()
		: SimulationLevel("texture billboard level")
	{
		m_texture_billboard_actor = CreateSPtr<TextureBillboardActor>(eMobilityState::kMovable);

		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<BillboardLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<BillboardLevelRuntimeNodeManager>();
	}

	bool TextureBillboardLevel::Initialize()
	{
		bool ret = SimulationLevel::Initialize();

		m_texture_billboard_actor->SetPosition(Vec3(0.f, 0.f, 500.f));
		SpawnActor(m_texture_billboard_actor);

		auto directional_light = CreateSPtr<DirectionalLight>();
		directional_light->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		directional_light->SetRotation(math::ToRadian(45.0f), 0.0f, 0.0f);
		directional_light->DisableShadow();
		SpawnActor(directional_light);

		return ret;
	}

	void TextureBillboardLevel::Shutdown()
	{
	}

	void TextureBillboardLevel::Update(float delta_time)
	{
	}

	void TextureBillboardLevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<TextureBillboardLevel>(shared_from_this()));
	}

	void TextureBillboardLevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void TextureBillboardLevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<TextureBillboardLevel>(shared_from_this()));
	}

	void TextureBillboardLevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}
}