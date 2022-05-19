#include <include/client_core.h>
#include <client/input/input.h>

#include "object/level/rotating_cube_level.h"
#include "object/actor/rotating_cube.h"

namespace simulation
{
	RotatingCubeLevel::RotatingCubeLevel()
		: Level("rotating cube level")
	{
		m_rotating_cube = CreateSPtr<RotatingCube>();
	}

	bool RotatingCubeLevel::Initialize()
	{
		SpawnActor(m_rotating_cube);
		m_rotating_cube->SetPosition(Vec3(0.f, 0.f, 500.f));

		Input::SetInputMode(eInputMode::kGameOnly);

		return true;
	}

	void RotatingCubeLevel::Shutdown()
	{
		Input::SetInputMode(eInputMode::kUIOnly);
	}

	void RotatingCubeLevel::Update(float delta_time)
	{
	}
}
