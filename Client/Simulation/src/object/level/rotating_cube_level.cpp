#include <include/client_core.h>
#include <client/input/input.h>
#include <client/object/actor/light.h>

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

		auto d_light = CreateSPtr<DirectionalLight>();
		d_light->SetLightColor(Vec3(1.0f, 1.0f, 1.0f));
		d_light->SetRotation(math::ToRadian(45.0f), 0.0f, 0.0f);
		d_light->DisableShadow();
		SpawnActor(d_light);

		return true;
	}

	void RotatingCubeLevel::Shutdown()
	{
	}

	void RotatingCubeLevel::Update(float delta_time)
	{
	}
}
