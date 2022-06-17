#include <include/client_core.h>
#include <client/object/actor/light.h>
#include <client/object/component/render/texture_billboard_component.h>

#include "object/actor/billboard_actor.h"

#include "object/level/rendering/basic/billboard_level.h"
#include "object/levelsetting/rendering/basic/billboard_level_node_manager.h"

namespace simulation
{
	UPtr<TextureBillboardLevelInitNodeManager> TextureBillboardLevel::s_init_node_manager = nullptr;
	UPtr<TextureBillboardLevelRuntimeNodeManager> TextureBillboardLevel::s_runtime_node_manager = nullptr;

	TextureBillboardLevel::TextureBillboardLevel()
		: SimulationLevel("texture billboard level")
	{
		m_texture_billboard_actor = CreateSPtr<TextureBillboardActor>(eMobilityState::kMovable);

		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<TextureBillboardLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<TextureBillboardLevelRuntimeNodeManager>();
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

	UPtr<TextureBillboardAnimationLevelInitNodeManager> TextureBillboardAnimationLevel::s_init_node_manager = nullptr;
	UPtr<TextureBillboardAnimationLevelRuntimeNodeManager> TextureBillboardAnimationLevel::s_runtime_node_manager = nullptr;

	TextureBillboardAnimationLevel::TextureBillboardAnimationLevel()
		: SimulationLevel("texture billboard animation level")
	{
		m_texture_billboard_actor = CreateSPtr<TextureBillboardActor>(eMobilityState::kMovable);

		if (s_init_node_manager == nullptr)
			s_init_node_manager = CreateUPtr<TextureBillboardAnimationLevelInitNodeManager>();
		if (s_runtime_node_manager == nullptr)
			s_runtime_node_manager = CreateUPtr<TextureBillboardAnimationLevelRuntimeNodeManager>();
	}

	bool TextureBillboardAnimationLevel::Initialize()
	{
		bool ret = SimulationLevel::Initialize();

		m_texture_billboard_actor->SetPosition(Vec3(0.f, 0.f, 500.f));
		m_texture_billboard_actor->GetTextureBillboardComponent()->SetTexture("Contents/Sprite/explosion.dds");
		SpawnActor(m_texture_billboard_actor);

		auto directional_light = CreateSPtr<DirectionalLight>();
		directional_light->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		directional_light->SetRotation(math::ToRadian(45.0f), 0.0f, 0.0f);
		directional_light->DisableShadow();
		SpawnActor(directional_light);

		return ret;
	}

	void TextureBillboardAnimationLevel::Shutdown()
	{
	}

	void TextureBillboardAnimationLevel::Update(float delta_time)
	{
		const auto& billboard_component = m_texture_billboard_actor->GetTextureBillboardComponent();
		if (s_runtime_node_manager->IsShowAnimation())
		{
			float time = m_texture_billboard_actor->GetAnimationTime();
			float period = s_runtime_node_manager->GetAnimationPeriod();

			time += delta_time;

			if (time >= period)
				time -= period;

			Vec2 tilling = billboard_component->GetTilling();
			Vec2 offset = billboard_component->GetOffset();
			offset.x = floorf(time / period / tilling.x / tilling.y) * tilling.x;
			offset.y = floorf(time / period / tilling.y) * tilling.y;
		
			billboard_component->SetOffset(offset);
			m_texture_billboard_actor->SetAnimationTime(time);
		}
	}

	void TextureBillboardAnimationLevel::SetLevelInitNodeOwner()
	{
		s_init_node_manager->SetOwner(std::static_pointer_cast<TextureBillboardAnimationLevel>(shared_from_this()));
	}

	void TextureBillboardAnimationLevel::ExecuteLevelInitNodes()
	{
		s_init_node_manager->ExecuteLevelSettingNodes();
	}

	void TextureBillboardAnimationLevel::SetLevelRuntimeNodeOwner()
	{
		s_runtime_node_manager->SetOwner(std::static_pointer_cast<TextureBillboardAnimationLevel>(shared_from_this()));
	}

	void TextureBillboardAnimationLevel::ExecuteLevelRuntimeNodes()
	{
		s_runtime_node_manager->ExecuteLevelSettingNodes();
	}
}