#pragma once

namespace client_fw
{
	struct Window;

	class RenderSystem;
	class TextRenderSystem;
	class FrameResourceManager;

	class Renderer final
	{
	public:
		Renderer(const WPtr<Window>& window);
		~Renderer();

		bool Initialize();
		void Shutdown();
		bool Update();
		bool Render();

		bool UpdateViewport();

	private:
		WPtr<Window> m_window;
		UPtr<RenderSystem> m_render_system;
		UPtr<TextRenderSystem> m_text_render_system;
		UPtr<FrameResourceManager> m_frame_resource_manager;
		bool m_is_level_changed = false;

	private:
		bool CreateDevice();
		bool CreateCommandObjects();
		bool CreateSwapchain();

		bool CreateRtvDsvHeaps();
		bool CreateRenderTargetViews();
		bool CreateDepthStencilView();
		bool ResizeViewport();

		void SetViewAndScissor(float left, float top, float width, float height);
		void FlushCommandQueue();

		bool InitializeRenderSystem();
		bool InitializeTextRenderSystem();
		bool InitializeFrameResourceManager();

	private:
		ComPtr<IDXGIFactory4> m_factory = nullptr;
		ComPtr<IDXGISwapChain3> m_swap_chain = nullptr;
		ComPtr<ID3D12Device> m_device = nullptr;

		static constexpr UINT s_swap_chain_buffer_count = 2;
		UINT m_cur_swapchain_buffer = 0;

		ComPtr<ID3D12Fence> m_fence = nullptr;
		UINT m_current_fence = 0;

		bool m_is_use_4x_mass = false;
		UINT m_4x_msaa_quality = 0;

		ComPtr<ID3D12CommandQueue> m_command_queue = nullptr;
		ComPtr<ID3D12CommandAllocator> m_command_allocator = nullptr;
		ComPtr<ID3D12GraphicsCommandList> m_command_list = nullptr;

		std::array<ComPtr<ID3D12Resource>, s_swap_chain_buffer_count> m_rtv_buffers;
		std::array<D3D12_CPU_DESCRIPTOR_HANDLE, s_swap_chain_buffer_count> m_rtv_cpu_handles;
		ComPtr<ID3D12DescriptorHeap> m_rtv_descriptor_heap = nullptr;

		ComPtr<ID3D12Resource> m_dsv_buffers = nullptr;
		D3D12_CPU_DESCRIPTOR_HANDLE m_dsv_cpu_handles;
		ComPtr<ID3D12DescriptorHeap> m_dsv_descriptor_heap = nullptr;

		D3D12_VIEWPORT m_viewport;
		D3D12_RECT m_scissor_rect;

		DXGI_FORMAT m_rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM;
		DXGI_FORMAT m_dsv_format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	public:
		const ComPtr<ID3D12Device>& GetDevice() const { return m_device; }
		const ComPtr<ID3D12GraphicsCommandList>& GetCommandList() const { return m_command_list; }
		const ComPtr<IDXGISwapChain3>& GetSwapChain() const { return m_swap_chain; }
		const ComPtr<ID3D12Resource>& GetCurrentRenderTarget() const { return m_rtv_buffers[m_cur_swapchain_buffer]; }
	};
}


