#pragma once
#include <client/object/level/core/level.h>

namespace simulation
{
	using namespace client_fw;

	class RotatingCube;

	class SimulationLevel : public Level
	{
	public:
		SimulationLevel(const std::string& name);
		virtual ~SimulationLevel() = default;

		virtual void ExecuteLevelInitNodes() {}
	};
}

