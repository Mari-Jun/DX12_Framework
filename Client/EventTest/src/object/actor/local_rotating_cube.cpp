#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/util/rotating_movement_component.h>
#include "local_rotating_cube.h"

namespace event_test
{
	LocalRotatingCube::LocalRotatingCube()
		: StaticMeshActor(eMobilityState::kMovable, "../Contents/bricks_cube.fbx")
	{
		m_rotating_component = CreateSPtr<RotatingMovementComponent>();
	}

	bool LocalRotatingCube::Initialize()
	{
		bool ret = StaticMeshActor::Initialize();
		ret &= AttachComponent(m_rotating_component);
		return ret;
	}

	void LocalRotatingCube::Shutdown()
	{
		StaticMeshActor::Shutdown();
	}

	void LocalRotatingCube::Update(float delta_time)
	{
	}

	void LocalRotatingCube::SetLocalPosition(const Vec3& position)
	{
		m_static_mesh_component->SetLocalPosition(position);
	}

	void LocalRotatingCube::SetRotatingSpeed(float speed)
	{
		m_rotating_component->SetRotatingRate(Vec3(0.0f, speed, 0.0f));
	}
}
