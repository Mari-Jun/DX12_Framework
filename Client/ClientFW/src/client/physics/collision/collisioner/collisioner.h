#pragma once
#include "client/physics/collision/mesh_bounding_tree.h"
#include "client/physics/collision/collision_util.h"

namespace client_fw
{
	class SceneComponent;
	class BoxComponent;
	class SphereComponent;
	class StaticMeshComponent;

	enum class eMeshCollisionType
	{
		kStaticMesh, kBox, kSphere, kRay
	};

	class Collisioner
	{
	public:
		Collisioner(eMeshCollisionType type);
		virtual ~Collisioner() = default;

		virtual bool CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other) = 0;
		virtual void BlockOtherComponent(const SPtr<SceneComponent>& other) = 0;

	protected:
		CollisionInfo m_collision_info;
		eMeshCollisionType m_type;
		WPtr<SceneComponent> m_owner;

	public:
		eMeshCollisionType GetMeshCollisionType() const { return m_type; }

		const CollisionInfo& GetCollisionInfo() const { return m_collision_info; }
		void SetCollisionInfo(CollisionInfo&& info) { m_collision_info = std::move(info); }
		void SetCollisionInfo(bool is_collision, bool is_blocking, bool generate_collision_event);
		void SetCollisionInfo(bool is_collision, bool is_blocking, std::string&& collision_type,
			std::set<std::string>&& collisionable_types, bool generate_collision_event);

		SPtr<SceneComponent> GetOwner() const { return m_owner.lock(); }
		void SetOwner(const SPtr<SceneComponent>& owner) { m_owner = owner; }
	};
}