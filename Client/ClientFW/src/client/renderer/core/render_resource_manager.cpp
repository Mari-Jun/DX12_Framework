#include "stdafx.h"
#include "client/object/level/core/level_manager.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/render_resource_frame_resource.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/material/material.h"
#include "client/asset/texture/texture.h"
#include "client/asset/texture/texture_loader.h"
#include "client/util/upload_buffer.h"

#include <client/imgui/imgui_impl_dx12.h>

namespace client_fw
{
	RenderResourceManager* RenderResourceManager::s_render_resource_manager = nullptr;

	RenderResourceManager::RenderResourceManager()
	{
		s_render_resource_manager = this;
	}

	RenderResourceManager::~RenderResourceManager()
	{
	}

	bool RenderResourceManager::Initialize(ID3D12Device* device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC texture_heap_desc;
		texture_heap_desc.NumDescriptors = MAX_2D_TEXTURE_RESOURCE_SIZE + MAX_CUBE_TEXTURE_RESOURCE_SIZE +
			MAX_ARRAY_TEXTURE_RESOURCE_SIZE + IMGUI_NUM_FRAMES_IN_FLIGHT + MAX_VIEWPORT_TEXTURE_RESOURCE_SIZE;
		texture_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		texture_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		texture_heap_desc.NodeMask = 0;

		if (FAILED(device->CreateDescriptorHeap(&texture_heap_desc, IID_PPV_ARGS(m_texture_desciptor_heap.GetAddressOf()))))
		{
			LOG_ERROR("Could not create texture descriptor heap");
			return false;
		}
		m_texture_desciptor_heap->SetName(L"Texture descriptor");

		LevelManager::GetLevelManager().AddLevelCloseEvent([this]() {
			});

		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(m_texture_desciptor_heap->GetCPUDescriptorHandleForHeapStart());
		cpu_handle.Offset(START_INDEX_IMGUI, D3DUtil::s_cbvsrvuav_descirptor_increment_size);
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle(m_texture_desciptor_heap->GetGPUDescriptorHandleForHeapStart());
		gpu_handle.Offset(START_INDEX_IMGUI, D3DUtil::s_cbvsrvuav_descirptor_increment_size);

		if (ImGui_ImplDX12_Init(device, IMGUI_NUM_FRAMES_IN_FLIGHT, DXGI_FORMAT_R8G8B8A8_UNORM,
			m_texture_desciptor_heap.Get(), cpu_handle, gpu_handle) == false)
		{
			LOG_ERROR("Could not initialize imgui DX12");
			return false;
		}
		
		m_texture_usage.fill(false);

		return true;
	}

	void RenderResourceManager::Shutdown()
	{
	}

	void RenderResourceManager::PreDraw(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		UpdateTextureResource(device, command_list);
		UpdateMaterialResource(device);
		UpdatePrimiviteResource(device, command_list);
	}

	void RenderResourceManager::Draw(ID3D12GraphicsCommandList* command_list) const
	{
		ID3D12DescriptorHeap* descriptor_heaps[] = { m_texture_desciptor_heap.Get() };
		command_list->SetDescriptorHeaps(_countof(descriptor_heaps), descriptor_heaps);

		const auto& render_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetRenderResourceFrameResource();

		if(m_materials.empty() == false)
			command_list->SetGraphicsRootShaderResourceView(3, render_resource->GetMaterialData()->GetResource()->GetGPUVirtualAddress());

		CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle(m_texture_desciptor_heap->GetGPUDescriptorHandleForHeapStart());
		command_list->SetGraphicsRootDescriptorTable(4, gpu_handle);
	}

	void RenderResourceManager::RegisterPrimitive(const SPtr<Primitive>& primitive)
	{
		m_ready_primitives.push_back(primitive);
	}

	void RenderResourceManager::RegisterMaterial(const SPtr<Material>& material)
	{
		m_materials.push_back(material);
	}

	void RenderResourceManager::RegisterTexture(const SPtr<Texture>& texture)
	{
		if (texture != nullptr)
		{
			switch (texture->GetTextureType())
			{
			case eTextureType::kExternal:
			{
				m_ready_external_textures.push_back(std::static_pointer_cast<ExternalTexture>(texture));
				break;
			}
			case eTextureType::kExternalCubeMap:
			{
				m_ready_external_cube_map_textures.push_back(std::static_pointer_cast<ExternalCubeMapTexture>(texture));
				break;
			}
			case eTextureType::kRedner:
			{
				m_ready_render_textures.push_back(std::static_pointer_cast<RenderTexture>(texture));
				break;
			}
			case eTextureType::kViewport:
			{
				m_ready_viewport_textures = std::static_pointer_cast<ViewportTexture>(texture);
				break;
			}
			case eTextureType::kRW:
			{
				m_ready_rw_textures.push_back(std::static_pointer_cast<RWTexture>(texture));
				break;
			}
			case eTextureType::kShadow:
			{
				m_ready_shadow_textures.push_back(std::static_pointer_cast<Shadow2DTexture>(texture));
				break;
			}
			case eTextureType::kShadowCubeMap:
			{
				m_ready_shadow_cube_textures.push_back(std::static_pointer_cast<ShadowCubeTexture>(texture));
				break;
			}
			case eTextureType::kShadowArray:
			{
				m_ready_shadow_array_textures.push_back(std::static_pointer_cast<ShadowArrayTexture>(texture));
				break;
			}
			case eTextureType::kRenderUI:
			{
				m_ready_render_text_textures.push_back(std::static_pointer_cast<RenderTextTexture>(texture));
				break;
			}
			default:
				break;
			}
		}
	}

	void RenderResourceManager::UpdatePrimiviteResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		for (const auto& primitive : m_ready_primitives)
			primitive->Initialize(device, command_list);
		m_ready_primitives.clear();
	}

	void RenderResourceManager::UpdateMaterialResource(ID3D12Device* device)
	{
		UINT new_size = static_cast<UINT>(m_materials.size());
		if (new_size > 0)
		{
			const auto& render_resource = FrameResourceManager::GetManager().GetCurrentFrameResource()->GetRenderResourceFrameResource();

			UINT material_size = render_resource->GetSizeOfMaterialData();
			bool is_need_create_resource = false;

			while (material_size <= new_size)
			{
				material_size = static_cast<UINT>(roundf(static_cast<float>(material_size) * 1.5f));
				is_need_create_resource = true;
			}

			if (is_need_create_resource)
			{
				render_resource->GetMaterialData()->CreateAndCopyResource(device, material_size);
				render_resource->SetSizeOfMaterialData(material_size);
			}

			if (new_size > render_resource->GetNumOfMaterial())
			{
				UINT index = render_resource->GetNumOfMaterial();

				for (auto iter = m_materials.cbegin() + index;
					iter != m_materials.cend(); ++iter)
				{
					const auto& material = *iter;

					material->SetResourceIndex(index);
					INT diffuse_index = -1;
					if (material->GetDiffuseTexture() != nullptr)
						diffuse_index = material->GetDiffuseTexture()->GetResourceIndex();
					INT normal_index = -1;
					if (material->GetNormalTexture() != nullptr)
						normal_index = material->GetNormalTexture()->GetResourceIndex();
					INT roughness_index = -1;
					INT metallic_index = -1;
					RSMaterialData data{ material->GetBaseColor(), material->GetRoughness(), material->GetMetallic(),
						diffuse_index, normal_index, roughness_index, metallic_index };

					render_resource->GetMaterialData()->CopyData(index++, data);
				}

				render_resource->SetNumOfMaterial(new_size);
			}
		}
	}

	void RenderResourceManager::UpdateTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		UpdateExternalTextureResource(device, command_list);
		UpdateRenderTextureResource(device, command_list);
		UpdateViewportTextureResource(device, command_list);
		UpdateRWTextureResource(device, command_list);
		UpdateShadowTextureResource(device, command_list);
		UpdateShadowCubeTextureResource(device, command_list);
		UpdateRenderTextTextureResource(device, command_list);
		UpdateExternalCubeMapTextureResource(device, command_list);
		UpdateShadowArrayTextureResource(device, command_list);
	}

	void RenderResourceManager::UpdateExternalTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		auto iter = m_texture_usage.begin();

		for (const auto& texture : m_ready_external_textures)
		{
			texture->Initialize(device, command_list);

			iter = std::find(iter, m_texture_usage.begin() + END_INDEX_EXTERNAL_TEXTURE, false);

			UpdateTextureResource<ExternalTexture>(iter, texture, [device]
			(auto index, const auto& texture, auto cpu_handle, auto gpu_handle) {
					device->CreateShaderResourceView(texture->GetResource(),
						&TextureCreator::GetShaderResourceViewDesc(texture->GetResource()), cpu_handle);
					texture->SetResourceIndex(index);
					texture->SetGPUHandle(gpu_handle);
					LOG_INFO("{0} - index [{1}]", texture->GetPath(), index);
				});
		}

		m_ready_external_textures.clear();
	}

	void RenderResourceManager::UpdateExternalCubeMapTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		auto iter = m_texture_usage.begin() + START_INDEX_EXTERNAL_CUBE_MAP_TEXTURE;

		for (const auto& texture : m_ready_external_cube_map_textures)
		{
			texture->Initialize(device, command_list);

			iter = std::find(iter, m_texture_usage.begin() + END_INDEX_EXTERNAL_CUBE_MAP_TEXTURE, false);

			UpdateTextureResource<ExternalCubeMapTexture>(iter, texture, [device]
			(auto index, const auto& texture, auto cpu_handle, auto gpu_handle) {
					device->CreateShaderResourceView(texture->GetResource(),
						&TextureCreator::GetShaderResourceViewDescForCube(texture->GetResource()), cpu_handle);
					texture->SetResourceIndex(index - START_INDEX_RENDER_CUBE_MAP_TEXTURE);
					LOG_INFO("{0} - index [{1}]", texture->GetPath(), index);
				});
		}

		m_ready_external_cube_map_textures.clear();
	}

	void RenderResourceManager::UpdateRenderTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		auto iter = m_texture_usage.begin() + START_INDEX_RENDER_TEXTURE;

		for (const auto& texture : m_ready_render_textures)
		{
			//GBuffer의 Format이 달라지게 된다면 변경이 필요하다.
			texture->Initialize(device, command_list,
				{ DXGI_FORMAT_R8G8B8A8_UNORM,  DXGI_FORMAT_R11G11B10_FLOAT, DXGI_FORMAT_R8G8B8A8_UNORM });

			if (texture->GetResourceIndex() >= 0)
			{
				CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle;

				for (UINT i = 0; i < texture->GetNumOfGBufferTexture(); ++i)
				{
					cpu_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_texture_desciptor_heap->GetCPUDescriptorHandleForHeapStart(),
						texture->GetGBufferResourceIndex(i), D3DUtil::s_cbvsrvuav_descirptor_increment_size);

					device->CreateShaderResourceView(texture->GetGBufferTexture(i),
						&TextureCreator::GetShaderResourceViewDesc(texture->GetGBufferTexture(i)), cpu_handle);
				}

				cpu_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_texture_desciptor_heap->GetCPUDescriptorHandleForHeapStart(),
					texture->GetResourceIndex(), D3DUtil::s_cbvsrvuav_descirptor_increment_size);
			
				device->CreateShaderResourceView(texture->GetResource(),
					&TextureCreator::GetShaderResourceViewDesc(texture->GetResource()), cpu_handle);

				cpu_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_texture_desciptor_heap->GetCPUDescriptorHandleForHeapStart(),
					texture->GetDSVResourceIndex(), D3DUtil::s_cbvsrvuav_descirptor_increment_size);

				device->CreateShaderResourceView(texture->GetDSVTexture(),
					&TextureCreator::GetShaderResourceViewDescForDSV(texture->GetDSVTexture()), cpu_handle);
			}
			else
			{
				for (UINT i = 0; i < texture->GetNumOfGBufferTexture(); ++i)
				{
					iter = std::find(iter, m_texture_usage.begin() + END_INDEX_RENDER_TEXTURE, false);
					
					UpdateTextureResource<RenderTexture>(iter, texture, [device, i]
					(auto index, const auto& texture, auto cpu_handle, auto gpu_handle) {
							device->CreateShaderResourceView(texture->GetGBufferTexture(i),
								&TextureCreator::GetShaderResourceViewDesc(texture->GetGBufferTexture(i)), cpu_handle);
							texture->SetGBufferResourceIndex(i, index);
							texture->SetGBufferGPUHandle(i, gpu_handle);
						});
				}

				iter = std::find(iter, m_texture_usage.begin() + END_INDEX_RENDER_TEXTURE, false);

				UpdateTextureResource<RenderTexture>(iter, texture, [device]
				(auto index, const auto& texture, auto cpu_handle, auto gpu_handle) {
						device->CreateShaderResourceView(texture->GetResource(),
							&TextureCreator::GetShaderResourceViewDesc(texture->GetResource()), cpu_handle);
						texture->SetResourceIndex(index);
						texture->SetGPUHandle(gpu_handle);
					});

				iter = std::find(iter, m_texture_usage.begin() + END_INDEX_RENDER_TEXTURE, false);
				UpdateTextureResource<RenderTexture>(iter, texture, [device]
				(auto index, const auto& texture, auto cpu_handle, auto gpu_handle) {
						device->CreateShaderResourceView(texture->GetDSVTexture(),
							&TextureCreator::GetShaderResourceViewDescForDSV(texture->GetDSVTexture()), cpu_handle);
						texture->SetDSVResourceIndex(index);
						texture->SetDSVGPUHandle(gpu_handle);
					});
			}
		}

		m_ready_render_textures.clear();
	}

	void RenderResourceManager::UpdateViewportTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(m_texture_desciptor_heap->GetCPUDescriptorHandleForHeapStart());
		cpu_handle.Offset(START_INDEX_VIEWPORT_TEXTURE, D3DUtil::s_cbvsrvuav_descirptor_increment_size);
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle(m_texture_desciptor_heap->GetGPUDescriptorHandleForHeapStart());
		gpu_handle.Offset(START_INDEX_VIEWPORT_TEXTURE, D3DUtil::s_cbvsrvuav_descirptor_increment_size);

		if (m_ready_viewport_textures != nullptr)
		{
			const auto& texture = m_ready_viewport_textures;

			texture->Initialize(device, command_list);

			device->CreateShaderResourceView(texture->GetResource(),
				&TextureCreator::GetShaderResourceViewDesc(texture->GetResource()), cpu_handle);
			texture->SetResourceIndex(START_INDEX_VIEWPORT_TEXTURE);
			texture->SetGPUHandle(gpu_handle);

			m_ready_viewport_textures = nullptr;
		}
	}

	void RenderResourceManager::UpdateRWTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		auto iter = m_texture_usage.begin() + START_INDEX_RENDER_TEXTURE;

		for (const auto& texture : m_ready_rw_textures)
		{
			texture->Initialize(device, command_list);

			if (texture->GetResourceIndex() >= 0)
			{
				CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle;

				cpu_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_texture_desciptor_heap->GetCPUDescriptorHandleForHeapStart(),
					texture->GetResourceIndex(), D3DUtil::s_cbvsrvuav_descirptor_increment_size);

				device->CreateShaderResourceView(texture->GetResource(),
					&TextureCreator::GetShaderResourceViewDesc(texture->GetResource()), cpu_handle);

				cpu_handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_texture_desciptor_heap->GetCPUDescriptorHandleForHeapStart(),
					texture->GetUAVResourceIndex(), D3DUtil::s_cbvsrvuav_descirptor_increment_size);

				device->CreateUnorderedAccessView(texture->GetResource(), nullptr,
					&TextureCreator::GetUnorderedAccessViewDesc(texture->GetResource()), cpu_handle);
			}
			else
			{
				iter = std::find(iter, m_texture_usage.begin() + END_INDEX_RENDER_TEXTURE, false);
				UpdateTextureResource<RWTexture>(iter, texture, [device]
				(auto index, const auto& texture, auto cpu_handle, auto gpu_handle) {
						device->CreateShaderResourceView(texture->GetResource(),
							&TextureCreator::GetShaderResourceViewDesc(texture->GetResource()), cpu_handle);
						texture->SetResourceIndex(index);
						texture->SetGPUHandle(gpu_handle);
					});

				iter = std::find(iter, m_texture_usage.begin() + END_INDEX_RENDER_TEXTURE, false);
				UpdateTextureResource<RWTexture>(iter, texture, [device]
				(auto index, const auto& texture, auto cpu_handle, auto gpu_handle) {
						device->CreateUnorderedAccessView(texture->GetResource(), nullptr,
							&TextureCreator::GetUnorderedAccessViewDesc(texture->GetResource()), cpu_handle);
						texture->SetUAVResourceIndex(index);
						texture->SetUAVGPUHandle(gpu_handle);
					});
			}
		}

		m_ready_rw_textures.clear();
	}

	void RenderResourceManager::UpdateShadowTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		auto iter = m_texture_usage.begin() + START_INDEX_RENDER_TEXTURE;

		for (const auto& texture : m_ready_shadow_textures)
		{
			texture->Initialize(device, command_list);

			iter = std::find(iter, m_texture_usage.begin() + END_INDEX_RENDER_TEXTURE, false);
			UpdateTextureResource<Shadow2DTexture>(iter, texture, [device]
			(auto index, const auto& texture, auto cpu_handle, auto gpu_handle) {
					device->CreateShaderResourceView(texture->GetResource(),
						&TextureCreator::GetShaderResourceViewDescFor32DSV(texture->GetResource()), cpu_handle);
					texture->SetResourceIndex(index);
					texture->SetGPUHandle(gpu_handle);
				});			
		}

		m_ready_shadow_textures.clear();
	}

	void RenderResourceManager::UpdateShadowCubeTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		auto iter = m_texture_usage.begin() + START_INDEX_RENDER_CUBE_MAP_TEXTURE;

		for (const auto& texture : m_ready_shadow_cube_textures)
		{
			texture->Initialize(device, command_list);

			iter = std::find(iter, m_texture_usage.begin() + END_INDEX_RENDER_CUBE_MAP_TEXTURE, false);
			UpdateTextureResource<ShadowCubeTexture>(iter, texture, [device]
			(auto index, const auto& texture, auto cpu_handle, auto gpu_handle) {
					device->CreateShaderResourceView(texture->GetResource(),
						&TextureCreator::GetShaderResourceViewDescFor32DSVCube(texture->GetResource()), cpu_handle);
					texture->SetResourceIndex(index - START_INDEX_RENDER_CUBE_MAP_TEXTURE);
				});
		}

		m_ready_shadow_cube_textures.clear();
	}

	void RenderResourceManager::UpdateShadowArrayTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		auto iter = m_texture_usage.begin() + START_INDEX_RENDER_ARRAY_TEXTURE;

		for (const auto& texture : m_ready_shadow_array_textures)
		{
			texture->Initialize(device, command_list);

			iter = std::find(iter, m_texture_usage.begin() + END_INDEX_RENDER_ARRAY_TEXTURE, false);
			UpdateTextureResource<ShadowArrayTexture>(iter, texture, [device]
			(auto index, const auto& texture, auto cpu_handle, auto gpu_handle) {
					device->CreateShaderResourceView(texture->GetResource(),
						&TextureCreator::GetShaderResourceViewDescFor32DSVArray(texture->GetResource(), texture->GetArraySize()), cpu_handle);
					texture->SetResourceIndex(index - START_INDEX_RENDER_ARRAY_TEXTURE);
				});		
		}

		m_ready_shadow_array_textures.clear();
	}

	void RenderResourceManager::UpdateRenderTextTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list)
	{
		auto iter = m_texture_usage.begin() + START_INDEX_RENDER_TEXT_TEXTURE;

		for (const auto& texture : m_ready_render_text_textures)
		{
			iter = std::find(iter, m_texture_usage.begin() + END_INDEX_RENDER_TEXT_TEXTURE, false);
			UpdateTextureResource<RenderTextTexture>(iter, texture, [device]
			(auto index, const auto& texture, auto cpu_handle, auto gpu_handle) {
					device->CreateShaderResourceView(texture->GetResource(),
						&TextureCreator::GetShaderResourceViewDesc(texture->GetResource()), cpu_handle);
					texture->SetResourceIndex(index);
				});
		}

		m_ready_render_text_textures.clear();
	}
}
