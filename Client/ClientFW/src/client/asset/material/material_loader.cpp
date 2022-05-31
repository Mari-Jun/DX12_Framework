#include "stdafx.h"
#include "client/asset/material/material_loader.h"
#include "client/asset/material/material.h"
#include "client/asset/texture/texture.h"
#include "client/asset/core/asset_manager.h"
#include "client/asset/core/asset_store.h"

namespace client_fw
{
	std::map<std::string, SPtr<Material>> MaterialLoader::LoadMaterials(const std::string& path, const std::string& extension) const
	{
		std::map<std::string, SPtr<Material>> materials;

		switch (HashCode(extension.c_str()))
		{
		case HashCode(".mtl"):
		{
			materials = LoadMtl(path, file_help::GetParentPathFromPath(path), extension);
			break;
		}
			
		default:
			LOG_ERROR("Files in {0} format cannot be supported", extension);
			break;
		} 

		return materials;
	}

	std::map<std::string, SPtr<Material>> MaterialLoader::LoadMtl(const std::string& path, const std::string& parent_path, const std::string& extension) const
	{
		std::ifstream mtl_file(path);

		std::map<std::string, SPtr<Material>> materials;

		if (mtl_file.is_open() == false)
		{
			LOG_ERROR("Could not find path : [{0}]", path);
		}
		else
		{
			std::stringstream ss;
			std::string line;
			std::string prefix;
			std::string temp_name;

			Vec4 temp_vec;

			bool is_new_mtl = false;
			SPtr<Material> material = nullptr;

			auto AddMaterial([&material, &materials, &is_new_mtl]() {
				if (material != nullptr)
				{
					materials.insert({ material->GetName(), std::move(material) });
					material = nullptr;
				}
				is_new_mtl = false;
				});


			while (std::getline(mtl_file, line))
			{
				ss.clear();
				prefix.clear();
				ss.str(line);
				ss >> prefix;

				switch (HashCode(prefix.c_str()))
				{
				case HashCode("newmtl"):
				{
					AddMaterial();
					ss >> temp_name;
					std::string mtl_path = parent_path + "/" + temp_name + extension;
					material = AssetStore::LoadMaterial(mtl_path);
					if (material == nullptr)
					{
						material = CreateSPtr<Material>();
						material->SetAssetInfo({ temp_name, mtl_path, extension });
						is_new_mtl = true;
					}
					break;
				}
				case HashCode("Kd"):
				{
					if (is_new_mtl)
					{
						ss >> temp_vec.x >> temp_vec.y >> temp_vec.z;
						temp_vec.w = 1.0f;
						material->SetBaseColor(temp_vec);
					}
					break;
				}
				case HashCode("map_Bump"):
				case HashCode("map_bump"):
				{
					if (is_new_mtl)
					{
						ss >> temp_name;
						std::string texture_path = parent_path + "/" + temp_name;
						SPtr<ExternalTexture> normal_texture = AssetStore::LoadTexture(texture_path);
						if (normal_texture != nullptr)
							material->SetNormalTexture(normal_texture);
					}
					break;
				}
				case HashCode("map_Kd"):
				{
					if (is_new_mtl)
					{
						ss >> temp_name;
						std::string texture_path = parent_path + "/" + temp_name;
						SPtr<ExternalTexture> diffuse_texture = AssetStore::LoadTexture(texture_path);
						if (diffuse_texture != nullptr)
							material->SetDiffuseTexture(diffuse_texture);
					}
					break;
				}
				default:
					break;
				}
			}

			AddMaterial();
		}

		return materials;
	}
}