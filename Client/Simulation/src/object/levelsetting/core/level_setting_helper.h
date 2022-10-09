#pragma once

namespace client_fw
{
	class Actor;
	//class Light;
	class DirectionalLight;
	class PointLight;
	class SpotLight;
}

namespace simulation
{
	using namespace client_fw;

	class LevelSettingHelper
	{
	public:
		static void ActorPositionFunc(const SPtr<Actor>& actor);
		static void ActorRotationFunc(const SPtr<Actor>& actor);
		static void ActorScaleFunc(const SPtr<Actor>& actor);

		static void DirectionalLightFunc(const SPtr<DirectionalLight>& light);
		static void PointLightFunc(const SPtr<PointLight>& light);
		static void SpotLightFunc(const SPtr<SpotLight>& light);
	};
}



