#pragma once
#include "object/level/core/simulation_level.h"

namespace client_fw
{
	class DirectionalLight;
}

namespace simulation
{
	using namespace client_fw;

	class RotatingCubeLevelInitNodeManager;
	class RotatingCubeLevelRuntimeNodeManager;

	class RotatingCube;

	class RotatingCubeLevel : public SimulationLevel
	{
	public:
		RotatingCubeLevel();
		virtual ~RotatingCubeLevel() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;

		virtual void SetLevelInitNodeOwner() override;
		virtual void ExecuteLevelInitNodes() override;

		virtual void SetLevelRuntimeNodeOwner() override;
		virtual void ExecuteLevelRuntimeNodes() override;

	private:
		SPtr<RotatingCube> m_rotating_cube;
		SPtr<DirectionalLight> m_directional_light;

	public:
		const SPtr<RotatingCube>& GetRotatingCube() const { return m_rotating_cube; }
		const SPtr<DirectionalLight>& GetDirectionalLight() const { return m_directional_light; }

	private:
		static UPtr<RotatingCubeLevelInitNodeManager> m_init_node_manager;
		static UPtr<RotatingCubeLevelRuntimeNodeManager> m_runtime_node_manager;
	};
}

