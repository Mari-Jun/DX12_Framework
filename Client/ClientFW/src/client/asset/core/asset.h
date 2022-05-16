#pragma once

namespace client_fw
{
	struct AssetInfo
	{
		std::string name;
		std::string path;
		std::string extension;
	};
	bool operator<(const AssetInfo& asset1, const AssetInfo& asset2);

	class Asset
	{
	public:
		virtual void Shutdown() {}

	protected:
		AssetInfo m_asset_info;

	public:
		const AssetInfo& GetAssetInfo() const { return m_asset_info; }
		void SetAssetInfo(const AssetInfo& asset_info) { m_asset_info = asset_info; }
		const std::string& GetName() const { return m_asset_info.name; }
		void SetName(const std::string& name) { m_asset_info.name = name; }
		const std::string& GetPath() const { return m_asset_info.path; }
		const std::string& GetExtension() const { return m_asset_info.extension; }
	};
}



