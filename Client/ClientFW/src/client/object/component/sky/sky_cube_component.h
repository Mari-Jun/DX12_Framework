#pragma once
#include "client/object/component/sky/sky_component.h"

namespace client_fw
{
	class ExternalCubeMapTexture;

	class SkyCubeComponent : public SkyComponent
	{
	public:
		SkyCubeComponent(const std::string& name = "sky cube component");

	private:
		SPtr<ExternalCubeMapTexture> m_cube_map_texture;

	public:
		const SPtr<ExternalCubeMapTexture>& GetCubeMapTexture() const { return m_cube_map_texture; }

		void SetCubeMapTexture(const std::string& path);
		void SetCubeMapTexture(const SPtr<ExternalCubeMapTexture>& cube_map_texture) { m_cube_map_texture = cube_map_texture; }
	};
}


