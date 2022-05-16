#include "stdafx.h"
#include "client/object/component/core/scene_component.h"
#include "client/object/actor/core/actor.h"
#include "client/util/octree/octree_manager.h"
#include "client/physics/collision/collisioner/collisioner.h"
#include "client/physics/core/actor_physics_manager.h"

namespace client_fw
{
	SceneComponent::SceneComponent(const std::string& name, int update_order)
		: Component(name, update_order)
		, m_local_position(vec3::ZERO), m_local_scale(Vec3(1.0f, 1.0f, 1.0f))
	{
	}

	SceneComponent::~SceneComponent()
	{
	}

	bool SceneComponent::InitializeComponent()
	{
		if (m_collisioner != nullptr)
			m_collisioner->SetOwner(shared_from_this());
		UpdateLocalMatrix();
		bool ret = Initialize();
		return ret;
	}

	void SceneComponent::ShutdownComponent()
	{
		UnregisterToCollsionOctree();
		Shutdown();
	}

	void SceneComponent::UpdateComponent(float delta_time)
	{
		m_is_updated_world_matrix = false;
		m_collided_components.clear();

		UpdateLocalMatrix();

		Update(delta_time);
	}

	void SceneComponent::UpdateWorldMatrix()
	{
		const auto& owner = m_owner.lock();
		if (owner != nullptr)
		{
			if (m_is_updated_world_matrix == false)
			{
				m_world_previous_position = m_world_position;
			}

			const auto& world = owner->GetWorldMatrix();
			m_world_matrix = m_local_matrix * world;
			m_world_position = vec3::TransformCoord(m_local_position, world);
			m_world_rotation = m_local_rotation * owner->GetRotation();
			m_world_scale = m_local_scale * owner->GetScale();
			UpdateOrientedBox();

			m_is_updated_world_matrix = true;

			ReregisterToCollisionOctree();
		}
	}

	void SceneComponent::UpdateLocalMatrix()
	{
		if (m_update_local_matrix)
		{
			m_local_matrix = mat4::CreateScale(m_local_scale);
			m_local_matrix *= mat4::CreateRotationFromQuaternion(m_local_rotation);
			m_local_matrix *= mat4::CreateTranslation(m_local_position);
			m_update_local_matrix = false;

			UpdateWorldMatrix();
		}
	}

	void SceneComponent::UpdateOrientedBox()
	{
	}

	void SceneComponent::RegisterToCollisionOctree()
	{
		if (m_collisioner != nullptr && m_owner.expired() == false)
		{
			const auto& info = m_collisioner->GetCollisionInfo();
			if (info.is_collision == true && info.collisionable_types.empty() == false)
				CollisionOctreeManager::GetOctreeManager().RegisterSceneComponent(shared_from_this());
		}
	}

	void SceneComponent::ReregisterToCollisionOctree()
	{
		if (m_collisioner != nullptr)
		{
			const auto& info = m_collisioner->GetCollisionInfo();
			if (info.is_collision == true && info.collisionable_types.empty() == false)
				CollisionOctreeManager::GetOctreeManager().ReregisterSceneComponent(shared_from_this());
		}
	}

	void SceneComponent::UnregisterToCollsionOctree()
	{
		if (m_collisioner != nullptr && m_owner.expired() == false)
		{
			const auto& info = m_collisioner->GetCollisionInfo();
			if (info.collisionable_types.empty() == false)
				CollisionOctreeManager::GetOctreeManager().UnregisterSceneComponent(shared_from_this());
		}
	}

	void SceneComponent::ExecuteCollisionResponse(const SPtr<SceneComponent>& comp,
		const SPtr<Actor>& other_actor, const SPtr<SceneComponent>& other_comp)
	{
		if (m_collision_responce_function != nullptr)
			m_collision_responce_function(comp, other_actor, other_comp);
	}

	void SceneComponent::SetLocalPosition(const Vec3& pos)
	{
		m_local_position = pos;
		m_update_local_matrix = true;
	}

	void SceneComponent::SetLocalRotation(const Quaternion& rot)
	{
		m_local_rotation = rot;
		m_update_local_matrix = true;
	}

	void SceneComponent::SetLocalRotation(float pitch, float yaw, float roll)
	{
		m_local_rotation = quat::CreateQuaternionFromRollPitchYaw(pitch, yaw, roll);
		m_update_local_matrix = true;
	}

	void SceneComponent::SetLocalScale(const Vec3& scale)
	{
		m_local_scale = scale;
		m_update_local_matrix = true;
	}

	void SceneComponent::SetLocalScale(float scale)
	{
		m_local_scale = Vec3(scale, scale, scale);
		m_update_local_matrix = true;
	}

	const UPtr<Collisioner>& SceneComponent::GetCollisioner() const
	{
		if (m_collisioner == nullptr)
			LOG_ERROR("Register component and call ""GetCollisioner"" function");
		return m_collisioner;
	}

	void SceneComponent::SetNoCollision()
	{
		if (m_collisioner != nullptr)
		{
			CollisionInfo info = m_collisioner->GetCollisionInfo();
			info.is_collision = false;
			m_collisioner->SetCollisionInfo(std::move(info));

			UnregisterToCollsionOctree();
		}
	}

	void SceneComponent::SetCollisionInfo(bool is_collision, bool is_blocking, bool generate_collsion_event)
	{
		if (m_collisioner != nullptr)
		{
			m_collisioner->SetCollisionInfo(is_collision, is_blocking, generate_collsion_event);

			if (is_collision)
				RegisterToCollisionOctree();
			else
				UnregisterToCollsionOctree();
		}
	}

	void SceneComponent::SetCollisionInfo(bool is_collision, bool is_blocking,
		std::string&& collision_type, std::set<std::string>&& collisionable_types, bool generate_collision_event)
	{
		if (m_collisioner != nullptr)
		{
			if (collisionable_types.empty())
				LOG_WARN("if collisionable_types empty, high possibility of errors during runtime. {0} - {1}",
					GetOwner().lock()->GetName(), GetName());

			m_collisioner->SetCollisionInfo(is_collision, is_blocking,
				std::move(collision_type), std::move(collisionable_types), generate_collision_event);

			if (is_collision)
				RegisterToCollisionOctree();
			else
				UnregisterToCollsionOctree();
		}
	}

	void SceneComponent::SetPhysics(bool value)
	{
		if (m_is_physics != value && m_collisioner != nullptr)
		{
			m_is_physics = value;
			m_owner.lock()->GetPhysicsManager()->SetActive(m_is_physics);
		}
	}

	void SceneComponent::AddCollisionTreeNode(const WPtr<CollisionTreeNode>& tree_node)
	{
		m_collision_tree_node.push_back(tree_node);
	}

	bool SceneComponent::IsCollidedComponent(const SPtr<SceneComponent>& component)
	{
		const auto& actor_name = component->GetOwner().lock()->GetName();
		if (m_collided_components.find(actor_name) == m_collided_components.cend())
			return false;
		else
		{
			auto& collided_components = m_collided_components[actor_name];
			if (collided_components.find(component->GetName()) == collided_components.cend())
				return false;
		}
		return true;
	}

	void SceneComponent::AddCollidedComponent(const SPtr<SceneComponent>& component)
	{
		if (GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
			m_collided_components[component->GetOwner().lock()->GetName()].insert(component->GetName());
	}
}
