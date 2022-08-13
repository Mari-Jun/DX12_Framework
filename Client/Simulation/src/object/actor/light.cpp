#include <include/client_core.h>
#include <client/object/component/util/rotating_movement_component.h>
#include <client/object/component/mesh/static_mesh_component.h>

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

	CubeSpotLight::CubeSpotLight(bool change_intensity)
		: StaticMeshActor(eMobilityState::kMovable, "../Contents/bricks_cube.obj")
		,m_update_intensity(change_intensity)
	{
		m_rotating_component = CreateSPtr<RotatingMovementComponent>();
		for (auto& spot_light : m_spot_lights)
			spot_light = CreateSPtr<SpotLightComponent>();
	}

	bool CubeSpotLight::Initialize()
	{
		bool ret = StaticMeshActor::Initialize();

		m_static_mesh_component->SetLocalScale(0.1f);
		m_rotating_component->SetRotatingRate(Vec3(180.f, 180.f, 0.0f));
		ret &= AttachComponent(m_rotating_component);
		Vec3 light_color;
		float yaw = 0.0f, pitch = 0.0f;
		for (int i = 0; i < 6; ++i)
		{
			switch (i)
			{
			case 0: light_color = Vec3(1.0f, 0.0f, 0.0f); yaw = 0.0f; break;
			case 1: light_color = Vec3(0.0f, 1.0f, 0.0f); yaw = 90.0f; break;
			case 2: light_color = Vec3(0.0f, 0.0f, 1.0f); yaw = 180.0f; break;
			case 3: light_color = Vec3(1.0f, 1.0f, 0.0f); yaw = 270.0f; break;
			case 4: light_color = Vec3(1.0f, 0.0f, 1.0f); pitch = 90.0f; break;
			case 5: light_color = Vec3(0.0f, 1.0f, 1.0f); pitch = -90.0f; break;
			default:
				break;
			}
			m_spot_lights[i]->SetLightColor(light_color);
			m_spot_lights[i]->SetLightIntensity(100'000.f);
			m_spot_lights[i]->SetLocalRotation(math::ToRadian(pitch), math::ToRadian(yaw), 0.0f);
			ret &= AttachComponent(m_spot_lights[i]);
		}

		return ret;
	}

	void CubeSpotLight::Shutdown()
	{
		m_rotating_component = nullptr;
		StaticMeshActor::Shutdown();
	}

	void CubeSpotLight::Update(float delta_time)
	{
		if (m_update_intensity)
		{
			for (auto& light : m_spot_lights)
			{
				float light_intensity = light->GetLightIntensity();
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

				light->SetLightIntensity(light_intensity);
			}
		}
	}
}