#pragma once

namespace client_fw
{
	class Texture;
	class ExternalTexture;
	class ExternalCubeMapTexture;

	class TextureLoader
	{
	public:
		TextureLoader() = default;

		TextureLoader(const TextureLoader&) = delete;
		TextureLoader& operator=(const TextureLoader&) = delete;

		virtual SPtr<ExternalTexture> LoadTexture(const std::string& path, const std::string& extension) const;
		virtual SPtr<ExternalCubeMapTexture> LoadCubeMapTexture(const std::string& path, const std::string& extension) const;
	};

	class TextureCreator
	{
	public:
		static ComPtr<ID3D12Resource> LoadTextureFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* command_list,
			const std::string& path, const std::string& extension, ComPtr<ID3D12Resource>& texture_upload_heap);

		static ComPtr<ID3D12Resource> Create2DTexture(ID3D12Device* device, DXGI_FORMAT format,
			const IVec2& size, UINT array_size, UINT mip_levels, D3D12_RESOURCE_FLAGS resource_flags, 
			D3D12_RESOURCE_STATES resource_states, D3D12_CLEAR_VALUE* clear_value);

	protected:
		static ComPtr<ID3D12Resource> LoadTextureFromDDSFile(ID3D12Device* device, ID3D12GraphicsCommandList* command_list,
			const std::wstring& path, ComPtr<ID3D12Resource>& texture_upload_heap);

		static ComPtr<ID3D12Resource> LoadTextureFromWICFile(ID3D12Device* device, ID3D12GraphicsCommandList* command_list,
			const std::wstring& path, ComPtr<ID3D12Resource>& texture_upload_heap);

	public:
		static D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDesc(const ComPtr<ID3D12Resource>& texture_resource);
		static D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDescForDSV(const ComPtr<ID3D12Resource>& dsv_resource);
		static D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDescFor32DSV(const ComPtr<ID3D12Resource>& dsv_resource);
		static D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDescFor32DSVArray(const ComPtr<ID3D12Resource>& dsv_resource, UINT array_size);
		static D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDescFor32DSVCube(const ComPtr<ID3D12Resource>& dsv_resource);
		static D3D12_SHADER_RESOURCE_VIEW_DESC GetShaderResourceViewDescForCube(const ComPtr<ID3D12Resource>& cube_map_resource);
		static D3D12_UNORDERED_ACCESS_VIEW_DESC GetUnorderedAccessViewDesc(const ComPtr<ID3D12Resource>& texture_resource);
	};
}


