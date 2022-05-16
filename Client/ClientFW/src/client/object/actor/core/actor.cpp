#include "stdafx.h"
#include "client/object/actor/core/actor.h"
#include "client/object/level/core/level_manager.h"
#include "client/object/component/core/component.h"
#include "client/object/component/core/component_manager.h"
#include "client/input/input.h"
#include "client/event/messageevent/message_helper.h"
#include "client/physics/core/actor_physics_manager.h"

namespace client_fw
{
	Actor::Actor(eMobilityState mobility, const std::string& name)
		: m_name(name), m_actor_state(eActorState::kActive)
		, m_mobility_state(mobility)
		, m_position(vec3::ZERO), m_scale(Vec3(1.0f, 1.0f, 1.0f))
	{
		m_component_manager = CreateUPtr<ComponentManager>();
		if (m_mobility_state == eMobilityState::kMovable)
		{
			UseUpdate();
			m_physics_manager = CreateUPtr<ActorPhysicsManager>();
		}
	}

	Actor::~Actor()
	{
	}

	bool Actor::InitializeActor()
	{
		if (m_physics_manager != nullptr)
			m_physics_manager->SetOwner(shared_from_this());

		UpdateWorldMatrix();
		bool ret = Initialize();
		UpdateWorldMatrix();
		return ret;
	}

	void Actor::ShutdownActor()
	{
		for (const auto& name : m_registered_input_events)
			Input::UnregisterInputEvent(name);
		for (const auto& name : m_registered_message_events)
		MessageHelper::UnregisterMessageReceiver(name, shared_from_this());

		m_component_manager->Shutdown();

		Shutdown();
	}

	void Actor::UpdateActor(float delta_time)
	{
		m_previous_position = m_position;
		m_is_updated_world_matrix = false;

		if (m_physics_manager != nullptr && m_physics_manager->IsActive())
			m_physics_manager->Update(delta_time);

		m_component_manager->Update(delta_time);
		Update(delta_time);

		UpdateWorldMatrix();
	}

	void Actor::UpdateWorldMatrix()
	{
		if (m_update_world_matrix)
		{
			m_world_matrix = mat4::CreateScale(m_scale);
			m_world_matrix *= mat4::CreateRotationFromQuaternion(m_rotation);
			m_world_matrix *= mat4::CreateTranslation(m_position);

			m_component_manager->UpdateWorldMatrix();
			
			m_is_updated_world_matrix = true;
			m_update_world_matrix = false;
		}
	}
	
	void Actor::SpawnActor(const SPtr<Actor>& actor)
	{
		LevelManager::GetLevelManager().SpawnActor(actor);
	}

	bool Actor::AttachComponent(const SPtr<Component> comp)
	{
		if (GetActorState() == eActorState::kDead)
			return false;
		comp->SetOwner(weak_from_this());
		return m_component_manager->RegisterComponent(comp);
	}

	void Actor::DetachComponent(const SPtr<Component> comp)
	{
		m_component_manager->UnregisterComponent(comp);
	}

	void Actor::RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption)
	{
		std::string event_name = m_name + " : " + name;
		if (Input::RegisterPressedEvent(event_name, std::move(keys), func, consumption, eInputOwnerType::kActor))
			RegisterInputEvent(event_name);
	}

	void Actor::RegisterReleasedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption)
	{
		std::string event_name = m_name + " : " + name;
		if (Input::RegisterReleasedEvent(event_name, std::move(keys), func, consumption, eInputOwnerType::kActor))
			RegisterInputEvent(event_name);
	}

	void Actor::RegisterAxisEvent(const std::string& name, std::vector<AxisEventKeyInfo>&& keys, 
		const std::function<bool(float)>& func, bool consumption)
	{
		std::string event_name = m_name + " : " + name;
		if (Input::RegisterAxisEvent(event_name, std::move(keys), func, consumption, eInputOwnerType::kActor))
			RegisterInputEvent(event_name);
	}

	void Actor::RegisterInputEvent(const std::string& name)
	{
		m_registered_input_events.push_back(name);
	}

	void Actor::RegisterReceiveMessage(UINT event_id)
	{
		MessageHelper::RegisterMessageReceiver(event_id, shared_from_this());
		m_registered_message_events.push_back(event_id);
	}

	void Actor::SetPosition(const Vec3& pos)
	{
		m_position = pos;
		m_update_world_matrix = true;
	}

	void Actor::SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
		m_update_world_matrix = true;
	}

	void Actor::SetRotation(float pitch, float yaw, float roll)
	{
		m_rotation = quat::CreateQuaternionFromRollPitchYaw(pitch, yaw, roll);
		m_update_world_matrix = true;
	}

	void Actor::SetScale(const Vec3& scale)
	{
		m_scale = scale;
		m_update_world_matrix = true;
	}

	void Actor::SetScale(float scale)
	{
		m_scale = Vec3(scale, scale, scale);
		m_update_world_matrix = true;
	}

}
