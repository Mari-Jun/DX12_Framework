#pragma once

namespace client_fw
{
	class Asset;
	class Mesh;
	class MeshLoader;
	class Material;
	class MaterialLoader;
	class ExternalTexture;
	class ExternalCubeMapTexture;
	class TextureLoader;

	enum class eAssetType
	{
		kMesh, kMaterial, kTexture, kAnimation
	};

	using AssetCache = std::unordered_map<std::string, SPtr<Asset>>;

	class AssetManager final
	{
	public:
		AssetManager();
		~AssetManager();
		
		void Initialize(UPtr<MeshLoader>&& mesh_loader, UPtr<MaterialLoader>&& material_loader, 
			UPtr<TextureLoader>&& texture_loader, bool level_cache = true);

		//void LoadAssets();
		//void LoadAssetsForLevel();

		void Shutdown();

	private:
		UPtr<MeshLoader> m_mesh_loader;
		UPtr<MaterialLoader> m_material_loader;
		UPtr<TextureLoader> m_texture_loader;

		bool m_is_level_cache;
		std::map<eAssetType, AssetCache> m_asset_caches;

	private:
		UINT GetNumOfAsset(eAssetType type) const;
		SPtr<Asset> LoadAsset(eAssetType type, const std::string& path) const;
		void SaveAsset(eAssetType type, const std::string& name, const std::string& path, 
			const std::string& extension, const SPtr<Asset>& asset);

	public:
		UINT GetNumOfMesh() const;
		SPtr<Mesh> LoadMesh(const std::string& path);
		SPtr<Material> LoadMaterial(const std::string& mtl_path);
		SPtr<Material> LoadMaterial(const std::string& path, const std::string& mtl_name);
		std::map<std::string, SPtr<Material>> LoadMaterials(const std::string& path);
		SPtr<ExternalTexture> LoadTexture(const std::string& path);
		SPtr<ExternalCubeMapTexture> LoadCubeMapTexture(const std::string& path);
	};

	namespace file_help
	{
		std::string GetStemFromPath(const std::string& path);
		std::string GetExtentionFromPath(const std::string& path);
		std::string GetParentPathFromPath(const std::string& path);
	}
}