#pragma once
#include <client/object/level/core/level.h>

namespace simulation
{
	using namespace client_fw;

	class RotatingCube;

	class RotatingCubeLevel : public Level
	{
	public:
		RotatingCubeLevel();
		virtual ~RotatingCubeLevel() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time);

	private:
		SPtr<RotatingCube> m_rotating_cube;
	};
}

