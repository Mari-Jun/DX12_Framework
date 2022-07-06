#pragma once

namespace client_fw
{
#define START_INDEX_EXTERNAL_TEXTURE			0			//Texture 15000개 까지 지원
#define END_INDEX_EXTERNAL_TEXTURE				15000

#define START_INDEX_RENDER_TEXTURE				15000		//Camera들 (Render, Shadow) Texture 3000개 까지 지원
#define END_INDEX_RENDER_TEXTURE				18000

#define START_INDEX_RENDER_TEXT_TEXTURE			18000		//Text Texture를 2000개 까지 지원
#define END_INDEX_RENDER_TEXT_TEXTURE			20000		

#define START_INDEX_RENDER_CUBE_MAP_TEXTURE		20000		//Render Cube Map Texture를 2000개까지 지원
#define END_INDEX_RENDER_CUBE_MAP_TEXTURE		22000

#define START_INDEX_EXTERNAL_CUBE_MAP_TEXTURE	22000		//External_Cube Map Texture를 500개까지 지원
#define END_INDEX_EXTERNAL_CUBE_MAP_TEXTURE		22500

#define START_INDEX_RENDER_ARRAY_TEXTURE		22500		//Render Cube Map Texture를 500개까지 지원
#define END_INDEX_RENDER_ARRAY_TEXTURE			23000

#define START_INDEX_IMGUI						23000
#define START_INDEX_VIEWPORT_TEXTURE			23003

#define MAX_2D_TEXTURE_RESOURCE_SIZE		20000
#define MAX_CUBE_TEXTURE_RESOURCE_SIZE		2500
#define MAX_ARRAY_TEXTURE_RESOURCE_SIZE		500
#define IMGUI_NUM_FRAMES_IN_FLIGHT			3
#define MAX_VIEWPORT_TEXTURE_RESOURCE_SIZE	1

	constexpr UINT TEXTURE_RESOURCE_COUNT = 30000;

	class Primitive;
	class Mesh;
	class Material;
	class Texture;
	class ExternalTexture;
	class ExternalCubeMapTexture;
	class RenderTexture;
	class ViewportTexture;
	class Shadow2DTexture;
	class ShadowArrayTexture;
	class ShadowCubeTexture;
	class RWTexture;
	class RenderTextTexture;
	enum class eTextureType;

	class RenderResourceManager final
	{
	public:
		RenderResourceManager();
		~RenderResourceManager();

		RenderResourceManager(const RenderResourceManager&) = delete;
		RenderResourceManager& operator=(const RenderResourceManager&) = delete;

		bool Initialize(ID3D12Device* device);
		void Shutdown();
		void PreDraw(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Draw(ID3D12GraphicsCommandList* command_list) const;

		void RegisterPrimitive(const SPtr<Primitive>& primitive);
		void RegisterMaterial(const SPtr<Material>& material);
		void RegisterTexture(const SPtr<Texture>& texture);

	private:
		void UpdatePrimiviteResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateMaterialResource(ID3D12Device* device);

		void UpdateTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		template<class TextureType>
		void UpdateTextureResource(std::array<bool, TEXTURE_RESOURCE_COUNT>::iterator iter, const SPtr<TextureType>& texture,
			std::function<void(int, const SPtr<TextureType>, CD3DX12_CPU_DESCRIPTOR_HANDLE, CD3DX12_GPU_DESCRIPTOR_HANDLE)> function)
		{
			UINT index = static_cast<UINT>(std::distance(m_texture_usage.begin(), iter));

			CD3DX12_CPU_DESCRIPTOR_HANDLE cpu_handle(m_texture_desciptor_heap->GetCPUDescriptorHandleForHeapStart(),
				index, D3DUtil::s_cbvsrvuav_descirptor_increment_size);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpu_handle(m_texture_desciptor_heap->GetGPUDescriptorHandleForHeapStart(),
				index, D3DUtil::s_cbvsrvuav_descirptor_increment_size);

			function(index, texture, cpu_handle, gpu_handle);

			texture->RegisterShutdownFunction([this, index]() {
				m_texture_usage[index] = false;
				});
			m_texture_usage[index] = true;
		}

		void UpdateExternalTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateExternalCubeMapTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateRenderTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateViewportTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateRWTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateShadowTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateShadowCubeTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateShadowArrayTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateRenderTextTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);

	private:
		static RenderResourceManager* s_render_resource_manager;

		std::vector<SPtr<Material>> m_materials;
		
		std::vector<SPtr<Primitive>> m_ready_primitives;
		std::vector<SPtr<ExternalTexture>> m_ready_external_textures;
		std::vector<SPtr<RenderTexture>> m_ready_render_textures;
		SPtr<ViewportTexture> m_ready_viewport_textures;
		std::vector<SPtr<RWTexture>> m_ready_rw_textures;
		std::vector<SPtr<Shadow2DTexture>> m_ready_shadow_textures;
		std::vector<SPtr<ShadowCubeTexture>> m_ready_shadow_cube_textures;
		std::vector<SPtr<ShadowArrayTexture>> m_ready_shadow_array_textures;
		std::vector<SPtr<RenderTextTexture>> m_ready_render_text_textures;
		std::vector<SPtr<ExternalCubeMapTexture>> m_ready_external_cube_map_textures;

	private:
		std::array<bool, TEXTURE_RESOURCE_COUNT> m_texture_usage;
		UINT m_num_of_external_texture_data = START_INDEX_EXTERNAL_TEXTURE;
		UINT m_num_of_external_cube_map_texture_data = START_INDEX_EXTERNAL_CUBE_MAP_TEXTURE;
		ComPtr<ID3D12DescriptorHeap> m_texture_desciptor_heap;

	public:
		static RenderResourceManager& GetRenderResourceManager() { return *s_render_resource_manager; }
		void ResetTextureUsage(UINT index) { m_texture_usage[std::clamp(index, 0u, TEXTURE_RESOURCE_COUNT - 1)] = false; }
	};
}

