#include <include/client_core.h>
#include <client/object/actor/sky_cube.h>

#include "simulation_level.h"

namespace simulation
{
	SimulationLevel::SimulationLevel(const std::string& name)
		: Level(name)
	{
		m_sky_cube = CreateSPtr<SkyCube>();
	}

	bool SimulationLevel::Initialize()
	{
		bool ret = true;
		m_sky_cube->SetTexturePath("Contents/SkyCube/dawn.dds");
		SpawnActor(m_sky_cube);
		return ret;
	}
}