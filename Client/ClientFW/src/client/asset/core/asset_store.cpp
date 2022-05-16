#include "stdafx.h"
#include "client/asset/core/asset_store.h"
#include "client/asset/core/asset_manager.h"
#include "client/asset/mesh/mesh.h"
#include "client/object/component/mesh/core/mesh_component.h"

namespace client_fw
{
	SPtr<Mesh> AssetStore::LoadMesh(const std::string& path)
	{
		return s_asset_manager->LoadMesh(path);
	}

	SPtr<Material> AssetStore::LoadMaterial(const std::string& mtl_path)
	{
		return s_asset_manager->LoadMaterial(mtl_path);
	}

	SPtr<Material> AssetStore::LoadMaterial(const std::string& path, const std::string& mtl_name)
	{
		return s_asset_manager->LoadMaterial(path, mtl_name);
	}

	std::map<std::string, SPtr<Material>> AssetStore::LoadMaterials(const std::string& path)
	{
		return s_asset_manager->LoadMaterials(path);
	}

	SPtr<ExternalTexture> AssetStore::LoadTexture(const std::string& path)
	{
		return s_asset_manager->LoadTexture(path);
	}

	SPtr<ExternalCubeMapTexture> AssetStore::LoadCubeMapTexture(const std::string& path)
	{
		return s_asset_manager->LoadCubeMapTexture(path);
	}
}
