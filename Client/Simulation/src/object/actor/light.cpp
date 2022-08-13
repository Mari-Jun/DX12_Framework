#include <include/client_core.h>

#include "object/actor/light.h"

namespace simulation
{
	IntensityPointLight::IntensityPointLight(eMobilityState mobility, const std::string& name)
		: PointLight(mobility, name)
	{
	}

	bool IntensityPointLight::Initialize()
	{
		bool ret = PointLight::Initialize();
		UseUpdate();
		return ret;
	}

	void IntensityPointLight::Update(float delta_time)
	{
		float light_intensity = GetLightIntensity();
		light_intensity += m_light_change_speed * delta_time;

		if (light_intensity > s_max_light_intensity)
		{
			m_light_change_speed *= -1.0f;
			light_intensity = s_max_light_intensity;
		}

		if (light_intensity < 0.0f)
		{
			m_light_change_speed *= -1.0f;
			light_intensity = 0.0f;
		}

		SetLightIntensity(light_intensity);
	}
}