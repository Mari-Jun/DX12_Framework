#include "stdafx.h"
#include "client/asset/texture/texture.h"
#include "client/asset/texture/texture_loader.h"
#include "client/util/dds_texture_loader12.h"
#include "client/util/wic_texture_loader12.h"

namespace client_fw
{
	SPtr<ExternalTexture> TextureLoader::LoadTexture(const std::string& path, const std::string& extension) const
	{
		SPtr<ExternalTexture> texture;

		if (std::filesystem::exists(path))
		{
			switch (HashCode(extension.c_str()))
			{
			case HashCode(".dds"):
			case HashCode(".png"):
			case HashCode(".jpg"):
			case HashCode(".bmp"):
				texture = CreateSPtr<ExternalTexture>();
				break;
			default:
				LOG_WARN("Files in {0} format cannot be supported", extension);
				break;
			}
		}
		else
		{
			LOG_WARN("file {0} does not exist.", path);
		}

		return texture;
	}

	SPtr<ExternalCubeMapTexture> TextureLoader::LoadCubeMapTexture(const std::string& path, const std::string& extension) const
	{
		SPtr<ExternalCubeMapTexture> texture;

		if (std::filesystem::exists(path))
		{
			if (extension == ".dds")
				texture = CreateSPtr<ExternalCubeMapTexture>();
			else
				LOG_WARN("Files in {0} format cannot be supported to cube map", extension);
		}

		return texture;
	}

	ComPtr<ID3D12Resource> TextureCreator::LoadTextureFromFile(ID3D12Device* device, ID3D12GraphicsCommandList* command_list,
		const std::string& path, const std::string& extension, ComPtr<ID3D12Resource>& texture_upload_heap)
	{
		ComPtr<ID3D12Resource> texture;

		std::wstring w_path(path.begin(), path.end());

		switch (HashCode(extension.c_str()))
		{
		case HashCode(".dds"):
			texture = TextureCreator::LoadTextureFromDDSFile(device, command_list, w_path, texture_upload_heap);
			break;
		case HashCode(".png"):
		case HashCode(".jpg"):
		case HashCode(".bmp"):
			texture = TextureCreator::LoadTextureFromWICFile(device, command_list, w_path, texture_upload_heap);
			break;
		default:
			break;
		}

		return texture;
	}

	ComPtr<ID3D12Resource> TextureCreator::Create2DTexture(ID3D12Device* device, DXGI_FORMAT format, 
		const IVec2& size, UINT array_size, UINT mip_levels, D3D12_RESOURCE_FLAGS resource_flags,
		D3D12_RESOURCE_STATES resource_states, D3D12_CLEAR_VALUE* clear_value)
	{
		ComPtr<ID3D12Resource> texture;

		D3D12_RESOURCE_DESC texture_desc;
		ZeroMemory(&texture_desc, sizeof(D3D12_RESOURCE_DESC));
		texture_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		texture_desc.Alignment = 0;
		texture_desc.Width = size.x;
		texture_desc.Height = size.y;
		texture_desc.DepthOrArraySize = array_size;
		texture_desc.MipLevels = mip_levels;
		texture_desc.Format = format;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		texture_desc.Flags = resource_flags;

		if (FAILED(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE, &texture_desc, resource_states,
			clear_value, IID_PPV_ARGS(texture.GetAddressOf()))))
		{
			LOG_ERROR("Could not create committed resource");
			return nullptr;
		}

		return texture;
	}

	ComPtr<ID3D12Resource> TextureCreator::LoadTextureFromDDSFile(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, 
		const std::wstring& path, ComPtr<ID3D12Resource>& texture_upload_heap)
	{
		ComPtr<ID3D12Resource> texture;

		UPtr<uint8_t[]> dds_data;
		std::vector<D3D12_SUBRESOURCE_DATA> sub_resources;

		if (FAILED(LoadDDSTextureFromFile(device, path.c_str(), texture.GetAddressOf(), dds_data, sub_resources)))
		{
			LOG_ERROR(L"Could not load dds texture : [{0}]", path);
			return nullptr;
		}

		const UINT num_sub_resources = static_cast<UINT>(sub_resources.size());
		const UINT64 upload_buffer_size = GetRequiredIntermediateSize(texture.Get(), 0, num_sub_resources);

		if (FAILED(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(upload_buffer_size),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&texture_upload_heap))))
		{
			LOG_ERROR("Could not create committed resource");
			return nullptr;
		}

		UpdateSubresources(command_list, texture.Get(), texture_upload_heap.Get(),
			0, 0, num_sub_resources, sub_resources.data());

		command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

		return texture;
	}

	ComPtr<ID3D12Resource> TextureCreator::LoadTextureFromWICFile(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, const std::wstring& path, ComPtr<ID3D12Resource>& texture_upload_heap)
	{
		ComPtr<ID3D12Resource> texture;

		UPtr<uint8_t[]> wic_data;
		D3D12_SUBRESOURCE_DATA sub_resource;

		if (FAILED(LoadWICTextureFromFile(device, path.c_str(), texture.GetAddressOf(), wic_data, sub_resource)))
		{
			LOG_ERROR(L"Could not load dds texture : [{0}]", path);
			return nullptr;
		}

		const UINT64 upload_buffer_size = GetRequiredIntermediateSize(texture.Get(), 0, 1);

		if (FAILED(device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(upload_buffer_size),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&texture_upload_heap))))
		{
			LOG_ERROR("Could not create committed resource");
			return nullptr;
		}

		UpdateSubresources(command_list, texture.Get(), texture_upload_heap.Get(),
			0, 0, 1, &sub_resource);

		command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

		return texture;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC TextureCreator::GetShaderResourceViewDesc(const ComPtr<ID3D12Resource>& texture_resource)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC view_desc;
		D3D12_RESOURCE_DESC resource_desc = texture_resource->GetDesc();

		view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		view_desc.Format = resource_desc.Format;

		switch (resource_desc.Dimension)
		{
		case D3D12_RESOURCE_DIMENSION_BUFFER:
		{
			view_desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
			view_desc.Buffer.FirstElement = 0;
			view_desc.Buffer.NumElements = 1;
			view_desc.Buffer.StructureByteStride = 0;
			view_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
			break;
		}
		case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
		{
			if (resource_desc.DepthOrArraySize == 1)
			{
				view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				view_desc.Texture2D.MipLevels = resource_desc.MipLevels;
				view_desc.Texture2D.MostDetailedMip = 0;
				view_desc.Texture2D.PlaneSlice = 0;
				view_desc.Texture2D.ResourceMinLODClamp = 0.0f;
			}
			else
			{
				view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
				view_desc.Texture2DArray.MipLevels = -1;
				view_desc.Texture2DArray.MostDetailedMip = 0;
				view_desc.Texture2DArray.PlaneSlice = 0;
				view_desc.Texture2DArray.ResourceMinLODClamp = 0.0f;
				view_desc.Texture2DArray.FirstArraySlice = 0;
				view_desc.Texture2DArray.ArraySize = resource_desc.DepthOrArraySize;
			}
			break;
		}
		}

		return view_desc;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC TextureCreator::GetShaderResourceViewDescForDSV(const ComPtr<ID3D12Resource>& dsv_resource)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC view_desc;

		view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		view_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MostDetailedMip = 0;
		view_desc.Texture2D.MipLevels = 1;
		view_desc.Texture2D.ResourceMinLODClamp = 0.0f;
		view_desc.Texture2D.PlaneSlice = 0;

		return view_desc;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC TextureCreator::GetShaderResourceViewDescFor32DSV(const ComPtr<ID3D12Resource>& dsv_resource)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC view_desc;

		view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		view_desc.Format = DXGI_FORMAT_R32_FLOAT;
		view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MostDetailedMip = 0;
		view_desc.Texture2D.MipLevels = 1;
		view_desc.Texture2D.ResourceMinLODClamp = 0.0f;
		view_desc.Texture2D.PlaneSlice = 0;

		return view_desc;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC TextureCreator::GetShaderResourceViewDescFor32DSVArray(const ComPtr<ID3D12Resource>& dsv_resource, UINT array_size)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC view_desc;

		view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		view_desc.Format = DXGI_FORMAT_R32_FLOAT;
		view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
		view_desc.Texture2DArray.FirstArraySlice = 0;
		view_desc.Texture2DArray.ArraySize = array_size;
		view_desc.Texture2DArray.MostDetailedMip = 0;
		view_desc.Texture2DArray.MipLevels = 1;
		view_desc.Texture2DArray.ResourceMinLODClamp = 0.0f;
		view_desc.Texture2DArray.PlaneSlice = 0;

		return view_desc;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC TextureCreator::GetShaderResourceViewDescFor32DSVCube(const ComPtr<ID3D12Resource>& dsv_resource)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC view_desc;

		view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		view_desc.Format = DXGI_FORMAT_R32_FLOAT;
		view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		view_desc.Texture2DArray.MipLevels = 1;
		view_desc.Texture2DArray.MostDetailedMip = 0;
		view_desc.Texture2DArray.ResourceMinLODClamp = 0.0f;

		return view_desc;
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC TextureCreator::GetShaderResourceViewDescForCube(const ComPtr<ID3D12Resource>& cube_map_resource)
	{
		D3D12_SHADER_RESOURCE_VIEW_DESC view_desc;
		D3D12_RESOURCE_DESC resource_desc = cube_map_resource->GetDesc();

		view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		view_desc.Format = resource_desc.Format;
		view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		view_desc.Texture2DArray.MipLevels = resource_desc.MipLevels;
		view_desc.Texture2DArray.MostDetailedMip = 0;
		view_desc.Texture2DArray.ResourceMinLODClamp = 0.0f;

		return view_desc;
	}

	D3D12_UNORDERED_ACCESS_VIEW_DESC TextureCreator::GetUnorderedAccessViewDesc(const ComPtr<ID3D12Resource>& texture_resource)
	{
		D3D12_UNORDERED_ACCESS_VIEW_DESC view_desc = {};
		D3D12_RESOURCE_DESC resource_desc = texture_resource->GetDesc();

		view_desc.Format = resource_desc.Format;
		view_desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
		view_desc.Texture2D.MipSlice = 0;

		return view_desc;
	}
}
