#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/util/projectile_movement_component.h>
#include "gravity_actor.h"

namespace event_test
{
	GravityActor::GravityActor(const std::string& path)
		: StaticMeshActor(eMobilityState::kMovable, path)
	{
		m_projectile_movement_component = CreateSPtr<ProjectileMovementComponent>();
	}

	bool GravityActor::Initialize()
	{
		bool ret = StaticMeshActor::Initialize();
		m_static_mesh_component->SetPhysics(true);
		//SetRotation(quat::CreateQuaternionFromNormal(vec3::AXIS_Y, math::ToRadian(180.0f)));
		SetRotation(math::ToRadian(-45.0f), math::ToRadian(90.0f), 0.0f);
		m_projectile_movement_component->SetInitialSpeed(1000.0f);
		ret &= AttachComponent(m_projectile_movement_component);
		return ret;
	}

	void GravityActor::Shutdown()
	{
	}

	void GravityActor::Update(float delta_time)
	{
	}
}