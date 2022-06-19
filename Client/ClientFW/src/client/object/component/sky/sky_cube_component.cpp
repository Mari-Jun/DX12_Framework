#include "stdafx.h"
#include "client/object/component/sky/sky_cube_component.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/texture/texture.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	SkyCubeComponent::SkyCubeComponent(const std::string& name)
		: SkyComponent(eSkyType::kCube, Render::ConvertShaderType(eShaderType::kSky), name)
	{
	}

	void SkyCubeComponent::SetCubeMapTexture(const std::string& path)
	{
		if (path.empty() == false)
		{
			const auto& texture = AssetStore::LoadCubeMapTexture(path);
			if (texture != nullptr)
				SetCubeMapTexture(texture);
			else
				LOG_ERROR("Could not find texture : {0}", path);
		}
		else
		{
			LOG_WARN("file path is empty");
		}
	}
}
