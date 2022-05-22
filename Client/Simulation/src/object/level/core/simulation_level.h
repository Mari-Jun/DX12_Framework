#pragma once
#include <client/object/level/core/level.h>

namespace client_fw
{
	class SkyCube;
}

namespace simulation
{
	using namespace client_fw;

	class RotatingCube;

	class SimulationLevel : public Level
	{
	public:
		SimulationLevel(const std::string& name);
		virtual ~SimulationLevel() = default;

		virtual bool Initialize() override;

		virtual void SetLevelInitNodeOwner() {}
		virtual void ExecuteLevelInitNodes() {}

		virtual void SetLevelRuntimeNodeOwner() {}
		virtual void ExecuteLevelRuntimeNodes() {}

	protected:
		SPtr<SkyCube> m_sky_cube;

	};
}

