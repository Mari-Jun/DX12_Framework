#pragma once
#include "object/level/core/simulation_level.h"

namespace client_fw
{
	class StaticMeshActor;
	class DirectionalLight;
	class PointLight;
	class SpotLight;
}

namespace simulation
{
	class LightLevelInitNodeManager;
	class LightLevelRuntimeNodeManager;

	class LightLevel : public SimulationLevel
	{
	public:
		LightLevel();
		virtual ~LightLevel() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;

		virtual void SetLevelInitNodeOwner() override;
		virtual void ExecuteLevelInitNodes() override;

		virtual void SetLevelRuntimeNodeOwner() override;
		virtual void ExecuteLevelRuntimeNodes() override;

	private:
		SPtr<DirectionalLight> m_directional_light1;
		SPtr<DirectionalLight> m_directional_light2;
		SPtr<PointLight> m_point_light1;
		SPtr<PointLight> m_point_light2;
		SPtr<PointLight> m_point_light3;
		SPtr<SpotLight> m_spot_light1;
		SPtr<SpotLight> m_spot_light2;
		SPtr<SpotLight> m_spot_light3;

	public:
		SPtr<DirectionalLight> GetDirectionalLight(int index) const;
		SPtr<PointLight> GetPointLight(int index) const;
		SPtr<SpotLight> GetSpotLight(int index) const;

	private:
		static UPtr<LightLevelInitNodeManager> s_init_node_manager;
		static UPtr<LightLevelRuntimeNodeManager> s_runtime_node_manager;
	};
}



