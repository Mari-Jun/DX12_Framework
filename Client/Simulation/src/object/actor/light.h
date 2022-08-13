#pragma once
#include <client/object/actor/light.h>

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
}