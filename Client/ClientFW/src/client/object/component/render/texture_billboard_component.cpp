#include "stdafx.h"
#include "client/object/component/render/texture_billboard_component.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/texture/texture.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	TextureBillboardComponent::TextureBillboardComponent(const std::string& name)
		: BillboardComponent(name, eBillboardType::kTexture, 
			Render::ConvertShaderType(eShaderType::kTextureBillboard))
	{
	}

	void TextureBillboardComponent::SetTexture(const std::string& path)
	{
		if (path.empty() == false)
		{
			SetTexture(AssetStore::LoadTexture(path));
			if (m_texture == nullptr)
				LOG_WARN("Could not find texture : {0}", path);
		}
		else
		{
			LOG_WARN("file path is empty");
		}
	}
}
