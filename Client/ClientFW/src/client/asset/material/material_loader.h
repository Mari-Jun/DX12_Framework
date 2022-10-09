#pragma once

struct aiScene;

namespace client_fw
{
	class Material;

	class MaterialLoader
	{
	public:
		MaterialLoader() = default;

		virtual std::map<std::string, SPtr<Material>> LoadMaterials(const std::string& path, const std::string& extension) const;

		virtual std::map<std::string, SPtr<Material>> LoadMtl(const std::string& path, const std::string& parent_path, const std::string& extension) const;

		virtual std::map<std::string, SPtr<Material>> LoadMaterialFromAssimp(const std::string& path, const aiScene* scene);
	};
}



