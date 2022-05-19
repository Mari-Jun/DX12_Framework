#include <include/client_core.h>
#include <client/object/component/mesh/static_mesh_component.h>
#include <client/object/component/util/rotating_movement_component.h>
#include "rotating_cube.h"

namespace simulation
{
	RotatingCube::RotatingCube()
		: StaticMeshActor(eMobilityState::kMovable, "../Contents/bricks_cube.obj")
	{
		m_rotating_component = CreateSPtr<RotatingMovementComponent>();
	}

	bool RotatingCube::Initialize()
	{
		bool ret = StaticMeshActor::Initialize();

		ret &= AttachComponent(m_rotating_component);

		return ret;
	}

	void RotatingCube::Shutdown()
	{
		m_rotating_component = nullptr;
		StaticMeshActor::Shutdown();
	}

	void RotatingCube::Update(float delta_time)
	{
	}
}