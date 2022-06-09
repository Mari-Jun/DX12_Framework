#pragma once
#include "client/asset/core/asset.h"

namespace client_fw
{
	enum class eTextureType
	{
		kExternal, kExternalCubeMap, 
		kRedner, kShadow, kShadowCubeMap, kShadowArray,
		kRenderUI, kViewport, kRW
	};

	class Texture
	{
	public:
		Texture(eTextureType type);
		virtual ~Texture();

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) { return true; }
		virtual bool Initialize(ID3D12Device* device) { return true; }

	protected:
		eTextureType m_texture_type;
		INT m_texture_resource_index = -1;

		ComPtr<ID3D12Resource> m_texture_resource;
		CD3DX12_GPU_DESCRIPTOR_HANDLE m_gpu_handle;

	public:
		eTextureType GetTextureType() const { return m_texture_type; }
		INT GetResourceIndex() const { return m_texture_resource_index; }
		void SetResourceIndex(INT index) { m_texture_resource_index = index; }
		ID3D12Resource* GetResource() const { return m_texture_resource.Get(); }
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUHandle() const { return m_gpu_handle; }
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress() const { return m_gpu_handle.ptr; }
		void SetGPUHandle(CD3DX12_GPU_DESCRIPTOR_HANDLE handle) { m_gpu_handle = handle; }
	};

	class ExternalTexture final: public Asset, public Texture
	{
	public:
		ExternalTexture();
		virtual ~ExternalTexture() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;

	private:
		ComPtr<ID3D12Resource> m_upload_heap;
	};

	class ExternalCubeMapTexture final : public Asset, public Texture
	{
	public:
		ExternalCubeMapTexture();
		virtual ~ExternalCubeMapTexture() = default;

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;

	private:
		ComPtr<ID3D12Resource> m_upload_heap;
	};

	// 카메라가 가지는 Texture로 
	// 카메라가 보는 장면을 그리는 Texture이다.
	// GBuffer Texture와 최종 render target view texture, depth sencil view texture가 있다.
	class RenderTexture : public Texture
	{
	public:
		RenderTexture(const IVec2& size);
		virtual ~RenderTexture();

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list, 
			const std::vector<DXGI_FORMAT>& gbuffer_rtv_formats);
		virtual bool Update(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		virtual void GBufferPreDraw(ID3D12GraphicsCommandList* command_list);
		virtual void GBufferPostDraw(ID3D12GraphicsCommandList* command_list);
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list);
		virtual void PostDraw(ID3D12GraphicsCommandList* command_list);

	private:
		bool CreateDescriptorHeaps(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void CreateGBufferAndRTVTexture(ID3D12Device* device, ID3D12GraphicsCommandList* command_list,
			const std::vector<DXGI_FORMAT>& gbuffer_rtv_formats);
		void CreateDSVTexture(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);

	private:
		IVec2 m_texture_size;
		UINT m_num_of_gbuffer_texture;
		std::vector<ComPtr<ID3D12Resource>> m_gbuffer_textures;
		std::vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_gbuffer_gpu_handle;
		ComPtr<ID3D12DescriptorHeap> m_rtv_descriptor_heap;
		std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_gbuffer_rtv_cpu_handles;
		std::vector<INT> m_gbuffer_texture_resource_indices;
		D3D12_CPU_DESCRIPTOR_HANDLE m_rtv_cpu_handle;

		ComPtr<ID3D12Resource> m_dsv_texture;
		CD3DX12_GPU_DESCRIPTOR_HANDLE m_dsv_gpu_handle;
		ComPtr<ID3D12DescriptorHeap> m_dsv_descriptor_heap;
		D3D12_CPU_DESCRIPTOR_HANDLE m_dsv_cpu_handle;
		INT m_dsv_texture_resource_index = -1;
		
	public:
		const IVec2& GetTextureSize() const { return m_texture_size; }
		void SetTextureSize(const IVec2& size) { m_texture_size = size; }
		UINT GetNumOfGBufferTexture() const { return m_num_of_gbuffer_texture; }
		ID3D12Resource* GetGBufferTexture(UINT buffer_index) const;
		INT GetGBufferResourceIndex(UINT buffer_index) const;
		void SetGBufferResourceIndex(UINT buffer_index, UINT resource_index);
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetGBufferGPUHandle(UINT buffer_index) const { return m_gbuffer_gpu_handle[buffer_index]; }
		D3D12_GPU_VIRTUAL_ADDRESS GetGBufferGPUAddress(UINT buffer_index) const { return m_gbuffer_gpu_handle[buffer_index].ptr; }
		void SetGBufferGPUHandle(UINT buffer_index, CD3DX12_GPU_DESCRIPTOR_HANDLE handle) { m_gbuffer_gpu_handle[buffer_index] = handle; }
		ID3D12Resource* GetDSVTexture() const { return m_dsv_texture.Get(); }
		INT GetDSVResourceIndex() const { return m_dsv_texture_resource_index; }
		void SetDSVResourceIndex(UINT index) { m_dsv_texture_resource_index = index; }
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetDSVGPUHandle() const { return m_dsv_gpu_handle; }
		D3D12_GPU_VIRTUAL_ADDRESS GetDSVGPUAddress() const { return m_dsv_gpu_handle.ptr; }
		void SetDSVGPUHandle(CD3DX12_GPU_DESCRIPTOR_HANDLE handle) { m_dsv_gpu_handle = handle; }
	};

	class ViewportTexture : public Texture
	{
	public:
		ViewportTexture(const IVec2& size);

	public:
		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list);
		virtual void PostDraw(ID3D12GraphicsCommandList* command_list);

	private:
		virtual bool CreateDescriptorHeaps(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void CreateRTVTexture(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);

	private:
		IVec2 m_texture_size;
		ComPtr<ID3D12DescriptorHeap> m_rtv_descriptor_heap;
		D3D12_CPU_DESCRIPTOR_HANDLE m_rtv_cpu_handle;

	public:
		const IVec2& GetTextureSize() const { return m_texture_size; }
	};

	class RWTexture : public Texture
	{
	public:
		RWTexture(const IVec2& size);
		virtual ~RWTexture();

		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list);
		virtual void PostDraw(ID3D12GraphicsCommandList* command_list);

	protected:
		IVec2 m_texture_size;
		INT m_uav_texture_resource_index = -1;
		CD3DX12_GPU_DESCRIPTOR_HANDLE m_uav_gpu_handle;

	public:
		const IVec2& GetTextureSize() const { return m_texture_size; }
		void SetTextureSize(const IVec2& size) { m_texture_size = size; }
		INT GetUAVResourceIndex() const { return m_uav_texture_resource_index; }
		void SetUAVResourceIndex(INT index) { m_uav_texture_resource_index = index; }
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetUAVGPUHandle() const { return m_uav_gpu_handle; }
		D3D12_GPU_VIRTUAL_ADDRESS GetUAVGPUAddress() const { return m_uav_gpu_handle.ptr; }
		void SetUAVGPUHandle(CD3DX12_GPU_DESCRIPTOR_HANDLE handle) { m_uav_gpu_handle = handle; }
	};

	class ShadowTexture : public Texture
	{
	protected:
		ShadowTexture(eTextureType type, const IVec2& size);
		virtual ~ShadowTexture();

	public:
		virtual bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;
		virtual void PreDraw(ID3D12GraphicsCommandList* command_list);
		virtual void PostDraw(ID3D12GraphicsCommandList* command_list);

	private:
		virtual bool CreateDescriptorHeaps(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		virtual void CreateDSVTexture(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) = 0;

	protected:
		IVec2 m_texture_size;
		ComPtr<ID3D12DescriptorHeap> m_dsv_descriptor_heap;
		D3D12_CPU_DESCRIPTOR_HANDLE m_dsv_cpu_handle;

	public:
		const IVec2& GetTextureSize() const { return m_texture_size; }
	};

	class Shadow2DTexture : public ShadowTexture
	{
	public:
		Shadow2DTexture(const IVec2& size);
		virtual ~Shadow2DTexture();

	private:
		virtual void CreateDSVTexture(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;
	};

	class ShadowArrayTexture : public ShadowTexture
	{
	public:
		ShadowArrayTexture(const IVec2& size, UINT array_size);
		virtual ~ShadowArrayTexture();

	private:
		virtual void CreateDSVTexture(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) override;

	private:
		UINT m_array_size = 1;

	public:
		UINT GetArraySize() const { return m_array_size; }
	};

	class ShadowCubeTexture : public ShadowArrayTexture
	{
	public:
		ShadowCubeTexture(const IVec2& size);
		virtual ~ShadowCubeTexture();
	};

	// Text를 저장하기 위한 Texture이다.
	// text는 DX12에서 지원하지 않기 때문에 dx11과 dwrite를 통해서 그린 후,
	// 텍스쳐화를 통해 DX12에서 그리는 방식을 사용하고 있다.
	//
	class RenderTextTexture final : public Texture
	{
	public:
		RenderTextTexture(const IVec2& size);
		virtual ~RenderTextTexture() = default;

		virtual bool Initialize(ID3D12Device* device) override;
		bool Initialize2D(ID3D11On12Device* device, ID2D1DeviceContext2* device_context);

	private:
		IVec2 m_texture_size;
		ComPtr<ID3D12DescriptorHeap> m_rtv_descriptor_heap;
		ComPtr<ID3D11Resource> m_wrapped_render_target;
		ComPtr<ID2D1Bitmap1> m_2d_render_target;

	public:
		ComPtr<ID3D11Resource> GetWrappedRenderTarget() const { return m_wrapped_render_target; }
		ID2D1Bitmap1* Get2DRenderTarget() const { return m_2d_render_target.Get(); }
	};
}


