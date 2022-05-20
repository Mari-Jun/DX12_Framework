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

	private:
		static UPtr<RotatingCubeLevelInitNodeManager> m_init_node_manager;
		static UPtr<RotatingCubeLevelRuntimeNodeManager> m_runtime_node_manager;
		Vec3 m_init_pos = Vec3(0.f, 0.f, 500.f);
		Vec3 m_init_scale = Vec3(1.f, 1.f, 1.f);
		bool m_enable_dir_light_shadow = false;

	public:
		void SetInitPos(const Vec3& pos) { m_init_pos = pos; }
		void SetInitScale(const Vec3& scale) { m_init_scale = scale; }
		void EnableDirectionalLightShadow(bool enable) { m_enable_dir_light_shadow = enable; }
	};
}

