#pragma once
#include "client/object/actor/core/actor.h"
#include "client/object/component/light/directional_light_component.h"
#include "client/object/component/light/point_light_component.h"
#include "client/object/component/light/spot_light_component.h"

namespace client_fw
{
	template <class Comp>
	class Light : public Actor
	{
	public:
		Light(eMobilityState mobility, const std::string& name)
			: Actor(mobility, name)
		{
			m_light_component = CreateSPtr<Comp>();
		}

		virtual ~Light() = default;

		virtual bool Initialize() override
		{
			bool ret = AttachComponent(m_light_component);
			return ret;
		}

		virtual void Shutdown() override
		{
			m_light_component = nullptr;
		}

	protected:
		SPtr<Comp> m_light_component;

	public:
		const SPtr<Comp>& GetLightComponent() const { return m_light_component; }

		const Vec3& GetLightColor() const
		{
			return m_light_component->GetLightColor();
		}
		void SetLightColor(const Vec3& color)
		{
			m_light_component->SetLightColor(color);
		}

		float GetLightIntensity() const
		{
			return m_light_component->GetLightIntensity();
		}
		void SetLightIntensity(float intensity)
		{
			m_light_component->SetLightIntensity(intensity);
		}

	public:
		void SetShadowTextureSize(INT extent)
		{
			m_light_component->SetShadowTextureSize(extent);
		}

		//초기화 과정에서 아예 Shadow를 사용하지 않는 경우만 사용
		bool IsUseShadow()
		{
			return m_light_component->IsUseShadow();
		}
		void DisableShadow()
		{
			m_light_component->DisableShadow();
		}

		// Rumtime On/Off
		bool GetShadowVisibility() const
		{
			return m_light_component->GetShadowVisibility();
		}
		void SetShadowVisibility(bool visible)
		{
			m_light_component->SetShadowVisibility(visible);
		}
	};

	class DirectionalLight : public Light<DirectionalLightComponent>
	{
	public:
		DirectionalLight(eMobilityState mobility = eMobilityState::kStatic,
			const std::string& name = "directional light");
		virtual ~DirectionalLight() = default;
	};

	class PointLight : public Light<PointLightComponent>
	{
	public:
		PointLight(eMobilityState mobility = eMobilityState::kStatic,
			const std::string& name = "point light");
		virtual ~PointLight() = default;

	public:
		float GetAttenuationRadius() const;
		void SetAttenuationRadius(float radius);
	};

	class SpotLight : public Light<SpotLightComponent>
	{
	public:
		SpotLight(eMobilityState mobility = eMobilityState::kStatic,
			const std::string& name = "spot light");
		virtual ~SpotLight() = default;

	public:
		float GetAttenuationRadius() const;
		void SetAttenuationRadius(float radius);
		float GetConeInnerAngle() const;
		void SetConeInnerAngle(float degree);
		float GetConeOuterAngle() const;
		void SetConeOuterAngle(float degree);
	};


}


