#pragma once

namespace client_fw
{
#define START_INDEX_EXTERNAL_TEXTURE			0			//Texture 15000개 까지 지원
#define START_INDEX_RENDER_TEXTURE				15000		//Camera들 (Render, Shadow) Texture 3000개 까지 지원
#define START_INDEX_RENDER_TEXT_TEXTURE			18000		//Text Texture를 2000개 까지 지원
#define START_INDEX_RENDER_CUBE_MAP_TEXTURE		20000		//Render Cube Map Texture를 2000개까지 지원
#define START_INDEX_EXTERNAL_CUBE_MAP_TEXTURE	22000		//External_Cube Map Texture를 500개까지 지원
#define START_INDEX_RENDER_ARRAY_TEXTURE		22500		//Render Cube Map Texture를 500개까지 지원
#define START_INDEX_IMGUI						23000

#define MAX_2D_TEXTURE_RESOURCE_SIZE		20000
#define MAX_CUBE_TEXTURE_RESOURCE_SIZE		2500
#define MAX_ARRAY_TEXTURE_RESOURCE_SIZE		500
#define IMGUI_NUM_FRAMES_IN_FLIGHT			3

	class Primitive;
	class Mesh;
	class Material;
	class Texture;
	class ExternalTexture;
	class ExternalCubeMapTexture;
	class RenderTexture;
	class Shadow2DTexture;
	class ShadowArrayTexture;
	class ShadowCubeTexture;
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
		void UpdateExternalTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateExternalCubeMapTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void UpdateRenderTextureResource(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
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
		std::vector<SPtr<Shadow2DTexture>> m_ready_shadow_textures;
		std::vector<SPtr<ShadowCubeTexture>> m_ready_shadow_cube_textures;
		std::vector<SPtr<ShadowArrayTexture>> m_ready_shadow_array_textures;
		std::vector<SPtr<RenderTextTexture>> m_ready_render_text_textures;
		std::vector<SPtr<ExternalCubeMapTexture>> m_ready_external_cube_map_textures;

	private:
		UINT m_num_of_external_texture_data = START_INDEX_EXTERNAL_TEXTURE;
		UINT m_num_of_external_cube_map_texture_data = START_INDEX_EXTERNAL_CUBE_MAP_TEXTURE;
		UINT m_num_of_render_texture_data = START_INDEX_RENDER_TEXTURE;
		UINT m_num_of_render_cube_map_texture_data = START_INDEX_RENDER_CUBE_MAP_TEXTURE;
		UINT m_num_of_render_array_texture_data = START_INDEX_RENDER_ARRAY_TEXTURE;
		UINT m_num_of_render_text_texture_data = START_INDEX_RENDER_TEXT_TEXTURE;
		ComPtr<ID3D12DescriptorHeap> m_texture_desciptor_heap;

	public:
		static RenderResourceManager& GetRenderResourceManager() { return *s_render_resource_manager; }
	};
}

