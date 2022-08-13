#pragma once
#include <client/object/actor/light.h>
#include <client/object/actor/static_mesh_actor.h>

namespace client_fw
{
	class RotatingMovementComponent;
}

namespace simulation
{
	using namespace client_fw;

	class IntensityPointLight : public PointLight
	{
	public:
		IntensityPointLight(eMobilityState mobility = eMobilityState::kStatic,
			const std::string& name = "intensity point light");
		virtual ~IntensityPointLight() = default;

		virtual bool Initialize() override;
		virtual void Update(float delta_time) override;

	private:
		constexpr static float s_max_light_intensity = 100'000.f;
		float m_light_change_speed = 50'000.f;
	};

	class CubeSpotLight : public StaticMeshActor
	{
	public:
		CubeSpotLight(bool change_intensity);
		virtual ~CubeSpotLight() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

		virtual void Update(float delta_time) override;

	private:
		SPtr<RotatingMovementComponent> m_rotating_component;
		std::array<SPtr<SpotLightComponent>, 6> m_spot_lights;
		bool m_update_intensity;
		constexpr static float s_max_light_intensity = 100'000.f;
		float m_light_change_speed = 50'000.f;

	public:
		const SPtr<RotatingMovementComponent>& GetRotatingComponent() const { return m_rotating_component; }
		const std::array<SPtr<SpotLightComponent>, 6>& GetSpotLights() const { return m_spot_lights; }
	};
}