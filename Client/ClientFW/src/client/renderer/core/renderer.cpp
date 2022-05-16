#include "stdafx.h"
#include "client/renderer/core/renderer.h"
#include "client/renderer/core/render_system.h"
#include "client/renderer/text/text_render_system.h"
#include "client/renderer/frameresource/core/frame_resource.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/core/window.h"
#include "client/util/d3d_util.h"
#include "client/object/level/core/level_manager.h"

namespace client_fw
{
	Renderer::Renderer(const WPtr<Window>& window)
		: m_window(window)
	{
		m_render_system = CreateUPtr<RenderSystem>(window);
		m_text_render_system = CreateUPtr<TextRenderSystem>();
		m_frame_resource_manager = CreateUPtr<FrameResourceManager>();
	}

	Renderer::~Renderer()
	{
	}

	bool Renderer::Initialize()
	{
		if (CreateDevice() == false)
		{
			LOG_ERROR("Could not create DX12 device");
			return false;
		}
		if (CreateCommandObjects() == false)
		{
			LOG_ERROR("Could not create DX12 command objects");
			return false;
		}
		if (CreateSwapchain() == false)
		{
			LOG_ERROR("Could not create DX12 swapchain");
			return false;
		}
		if (CreateRtvDsvHeaps() == false)
		{
			LOG_ERROR("Could not create DX12 heaps");
			return false;
		}
		if (ResizeViewport() == false)
		{
			LOG_ERROR("Could not resize window");
			return false;
		}
		if (InitializeRenderSystem() == false)
		{
			LOG_ERROR("Could not initialize render system");
			return false;
		}
		if (InitializeTextRenderSystem() == false)
		{
			LOG_ERROR("Could not initialize text render system");
			return false;
		}
		if (InitializeFrameResourceManager() == false)
		{
			LOG_ERROR("Could not initialize frame resource manager");
			return false;
		}

		LevelManager::GetLevelManager().AddLevelCloseEvent([this]() {
			m_is_level_changed = true;
			});
		
		return true;
	}

	void Renderer::Shutdown()
	{
		//WaitForGpuCompelete();

		//CloseHandle(m_fence_event);

		FlushCommandQueue();
		
		m_frame_resource_manager->Shutdown();
		m_text_render_system->Shutdown();
		m_render_system->Shutdown();
	}

	bool Renderer::InitializeRenderSystem()
	{
		m_command_list->Reset(m_command_allocator.Get(), nullptr);

		if (m_render_system->Initialize(m_device.Get(), m_command_list.Get()))
		{
			m_command_list->Close();
			ID3D12CommandList* cmd_lists[] = { m_command_list.Get() };
			m_command_queue->ExecuteCommandLists(_countof(cmd_lists), cmd_lists);

			//WaitForGpuCompelete();
			FlushCommandQueue();

			return true;
		}
		return false;
	}

	bool Renderer::InitializeTextRenderSystem()
	{
		if (m_text_render_system->Initialize(m_device.Get(), m_command_queue.Get()) == false)
		{
			LOG_ERROR("Could not initialize ui render system");
			return false;
		}

		return true;
	}

	bool Renderer::InitializeFrameResourceManager()
	{
		return m_frame_resource_manager->Initialize(m_device.Get());
	}

	bool Renderer::Render()
	{
		if (m_is_level_changed)
		{
			const auto& frame_resource = m_frame_resource_manager->GetCurrentFrameResource();

			if (frame_resource->GetFence() != 0 &&
				m_fence->GetCompletedValue() < frame_resource->GetFence())
			{
				HANDLE event_handle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
				if (FAILED(m_fence->SetEventOnCompletion(frame_resource->GetFence(), event_handle)))
				{
					LOG_WARN("Failed to reach fence value");
					return false;
				}
				WaitForSingleObject(event_handle, INFINITE);
				CloseHandle(event_handle);
			}
			m_is_level_changed = false;
		}
		else
		{
			m_frame_resource_manager->MoveToNextFrame();

			const auto& frame_resource = m_frame_resource_manager->GetCurrentFrameResource();

			if (frame_resource->GetFence() != 0 &&
				m_fence->GetCompletedValue() < frame_resource->GetFence())
			{
				HANDLE event_handle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
				if (FAILED(m_fence->SetEventOnCompletion(frame_resource->GetFence(), event_handle)))
				{
					LOG_WARN("Failed to reach fence value");
					return false;
				}
				WaitForSingleObject(event_handle, INFINITE);
				CloseHandle(event_handle);
			}

			m_text_render_system->Update(m_device.Get());
			m_render_system->Update(m_device.Get());

			const auto& allocator = frame_resource->GetCommandAllocator();

			if (FAILED(allocator->Reset()))
			{
				LOG_ERROR("Could not reset command allocator");
				return false;
			}

			if (FAILED(m_command_list->Reset(allocator.Get(), nullptr)))
			{
				LOG_ERROR("Could not reset command list");
				return false;
			}

			m_render_system->PreDraw(m_device.Get(), m_command_list.Get());
			m_text_render_system->Draw();
			m_render_system->Draw(m_command_list.Get());

			m_command_list->RSSetViewports(1, &m_viewport);
			m_command_list->RSSetScissorRects(1, &m_scissor_rect);

			m_command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentRenderTarget().Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

			m_command_list->OMSetRenderTargets(1, &m_rtv_cpu_handles[m_cur_swapchain_buffer], true, &m_dsv_cpu_handles);
			m_command_list->ClearRenderTargetView(m_rtv_cpu_handles[m_cur_swapchain_buffer], Colors::Black, 0, nullptr);
			m_command_list->ClearDepthStencilView(m_dsv_cpu_handles,
				D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

			m_render_system->DrawMainCameraView(m_command_list.Get());
			m_render_system->DrawUI(m_command_list.Get());

			m_command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentRenderTarget().Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
			m_text_render_system->PostDraw(m_command_list.Get());

			if (FAILED(m_command_list->Close()))
			{
				LOG_ERROR("Could not close command list");
				return false;
			}

			ID3D12CommandList* cmd_lists[] = { m_command_list.Get() };
			m_command_queue->ExecuteCommandLists(_countof(cmd_lists), cmd_lists);

			m_swap_chain->Present(0, 0);
			m_cur_swapchain_buffer = (m_cur_swapchain_buffer + 1) % s_swap_chain_buffer_count;

			frame_resource->SetFence(++m_current_fence);

			m_command_queue->Signal(m_fence.Get(), m_current_fence);
		}

		return true;
	}

	bool Renderer::UpdateViewport()
	{
		if (m_device != nullptr)
		{
			if (ResizeViewport() == false)
			{
				LOG_ERROR("Could not resize window");
				return false;
			}
			m_render_system->UpdateViewport();
		}
		return true;
	}

	bool Renderer::CreateDevice()
	{
		HRESULT result;
		UINT factory_flags = 0;

#if defined(DEBUG) || defined(_DEBUG)
		{
			ComPtr<ID3D12Debug> debug_controller;
			result = D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller));
			if (FAILED(result)) return false;

			debug_controller->EnableDebugLayer();
			factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
		}
#endif

		result = CreateDXGIFactory2(factory_flags, IID_PPV_ARGS(&m_factory));
		if (FAILED(result))
		{
			LOG_ERROR("Could not create DXGIFactory");
			return false;
		}
		D3DUtil::SetObjectName(m_factory.Get(), "renderer_factory");

		ComPtr<IDXGIAdapter1> adapter;
		for (UINT i = 0; i < DXGI_ERROR_NOT_FOUND != m_factory->EnumAdapters1(i, adapter.GetAddressOf()); ++i)
		{
			DXGI_ADAPTER_DESC1 adapter_desc;
			adapter->GetDesc1(&adapter_desc);
			if (adapter_desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device)))) break;
		}

		if (adapter == nullptr)
		{
			m_factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter));
			result = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
			if (FAILED(result))
			{
				LOG_ERROR("Could not create D3D12Device");
				return false;
			}
		}
		D3DUtil::SetObjectName(m_device.Get(), "renderer_device");

		result = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
		if (FAILED(result))
		{
			LOG_ERROR("Could not create fence");
			return false;
		}
		D3DUtil::SetObjectName(m_fence.Get(), "renderer_fence");

		D3DUtil::s_cbvsrvuav_descirptor_increment_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		D3DUtil::s_rtv_descirptor_increment_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3DUtil::s_dsv_descirptor_increment_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS ms_quality_levels;
		ms_quality_levels.Format = m_rtv_format;
		ms_quality_levels.SampleCount = 4;
		ms_quality_levels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
		ms_quality_levels.NumQualityLevels = 0;
		result = m_device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &ms_quality_levels, sizeof(ms_quality_levels));
		if (FAILED(result))
		{
			LOG_ERROR("Could not support 4x MSAA");
			return false;
		}
		m_4x_msaa_quality = ms_quality_levels.NumQualityLevels;
		m_is_use_4x_mass = (m_4x_msaa_quality > 1) ? true : false;
		D3DUtil::s_is_use_4x_mass = m_is_use_4x_mass;
		D3DUtil::s_4x_msaa_quality = m_4x_msaa_quality;

		return true;
	}

	bool Renderer::CreateCommandObjects()
	{
		D3D12_COMMAND_QUEUE_DESC queue_desc = {};
		queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		if (FAILED(m_device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&m_command_queue))))
		{
			LOG_ERROR("Could not create command queue");
			return false;
		}
		if (FAILED(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command_allocator))))
		{
			LOG_ERROR("Could not create command allocator");
			return false;
		}
		if (FAILED(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_command_allocator.Get(), nullptr, IID_PPV_ARGS(&m_command_list))))
		{
			LOG_ERROR("Could not create command list");
			return false;
		}
		D3DUtil::SetObjectName(m_command_queue.Get(), "renderer_command_queue");
		D3DUtil::SetObjectName(m_command_allocator.Get(), "renderer_command_allocator");
		D3DUtil::SetObjectName(m_command_list.Get(), "renderer_command_list");
		m_command_list->Close();

	
		return true;
	}

	bool Renderer::CreateSwapchain()
	{
		const auto& window = m_window.lock();

		//만약 resize가 필요 없는 상태라면
		/*m_swap_chain.Reset();

		DXGI_SWAP_CHAIN_DESC sc_desc;
		sc_desc.BufferDesc.Width = window->width;
		sc_desc.BufferDesc.Height = window->height;
		sc_desc.BufferDesc.Format = m_rtv_format;
		sc_desc.BufferDesc.RefreshRate.Numerator = 60;
		sc_desc.BufferDesc.RefreshRate.Denominator = 1;
		sc_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sc_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sc_desc.SampleDesc.Count = m_is_use_4x_mass ? 4 : 1;
		sc_desc.SampleDesc.Quality = m_is_use_4x_mass ? m_4x_msaa_quality - 1 : 0;
		sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sc_desc.BufferCount = s_swap_chain_buffer_count;
		sc_desc.OutputWindow = window->hWnd;
		sc_desc.Windowed = true;
		sc_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sc_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		if (FAILED(m_factory->CreateSwapChain(m_command_queue.Get(), &sc_desc,
			reinterpret_cast<IDXGISwapChain**>(m_swap_chain.GetAddressOf()))))
			return false;*/

		DXGI_SWAP_CHAIN_DESC1 sc_desc = {};
		sc_desc.BufferCount = s_swap_chain_buffer_count;
		sc_desc.Width = window->width;
		sc_desc.Height = window->height;
		sc_desc.Format = m_rtv_format;
		sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sc_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sc_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		sc_desc.SampleDesc.Count = 1;


		ComPtr<IDXGISwapChain1> swap_chain;
		if (FAILED(m_factory->CreateSwapChainForHwnd(m_command_queue.Get(), 
			window->hWnd, &sc_desc, nullptr, nullptr, &swap_chain)))
			return false;
		if (FAILED(swap_chain.As(&m_swap_chain)))
			return false;


		m_factory->MakeWindowAssociation(window->hWnd, DXGI_MWA_NO_ALT_ENTER);
		
		

		D3DUtil::SetObjectName(m_swap_chain.Get(), "renderer_swap_chain");
		m_cur_swapchain_buffer = m_swap_chain->GetCurrentBackBufferIndex();

		return true;
	}

	bool Renderer::CreateRtvDsvHeaps()
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc;
		rtv_heap_desc.NumDescriptors = s_swap_chain_buffer_count;
		rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtv_heap_desc.NodeMask = 0;
		if (FAILED(m_device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&m_rtv_descriptor_heap))))
		{
			LOG_ERROR("Could not create RTV descriptor heap");
			return false;
		}
		D3DUtil::SetObjectName(m_rtv_descriptor_heap.Get(), "renderer_rtv_heap");
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_heap_handle(m_rtv_descriptor_heap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < s_swap_chain_buffer_count; ++i)
		{
			m_rtv_cpu_handles[i] = rtv_heap_handle;
			rtv_heap_handle.Offset(1, D3DUtil::s_rtv_descirptor_increment_size);
		}

		D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc;
		dsv_heap_desc.NumDescriptors = 1;
		dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsv_heap_desc.NodeMask = 0;
		if (FAILED(m_device->CreateDescriptorHeap(&dsv_heap_desc, IID_PPV_ARGS(&m_dsv_descriptor_heap))))
		{
			LOG_ERROR("Could not create DSV descriptor heap");
			return false;
		}
		D3DUtil::SetObjectName(m_dsv_descriptor_heap.Get(), "renderer_dsv_heap");
		m_dsv_cpu_handles = m_dsv_descriptor_heap->GetCPUDescriptorHandleForHeapStart();

		return true;
	}

	bool Renderer::CreateRenderTargetViews()
	{
		for (UINT i = 0; i < s_swap_chain_buffer_count; ++i)
		{
			if (FAILED(m_swap_chain->GetBuffer(i, IID_PPV_ARGS(&m_rtv_buffers[i]))))
			{
				LOG_ERROR("Could not get swap chain buffer [" + std::to_string(i) + "]");
				return false;
			}
			D3DUtil::SetObjectName(m_rtv_buffers[i].Get(), "renderer_rtv_buffer" + std::to_string(i));
			m_device->CreateRenderTargetView(m_rtv_buffers[i].Get(), nullptr, m_rtv_cpu_handles[i]);
		}
		return true;
	}

	bool Renderer::CreateDepthStencilView()
	{
		D3D12_RESOURCE_DESC ds_desc;
		ds_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		ds_desc.Alignment = 0;
		ds_desc.Width = m_window.lock()->width;
		ds_desc.Height = m_window.lock()->height;
		ds_desc.DepthOrArraySize = 1;
		ds_desc.MipLevels = 1;
		ds_desc.Format = m_dsv_format;
		ds_desc.SampleDesc.Count = m_is_use_4x_mass ? 4 : 1;
		ds_desc.SampleDesc.Quality = m_is_use_4x_mass ? m_4x_msaa_quality - 1 : 0;
		ds_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		ds_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		D3D12_CLEAR_VALUE clear_vaule;
		clear_vaule.Format = m_dsv_format;
		clear_vaule.DepthStencil.Depth = 1.0f;
		clear_vaule.DepthStencil.Stencil = 0;

		if (FAILED(m_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
			&ds_desc, D3D12_RESOURCE_STATE_COMMON, &clear_vaule, IID_PPV_ARGS(&m_dsv_buffers))))
		{
			LOG_ERROR("Could not create dsv resource");
			return false;
		}
		D3DUtil::SetObjectName(m_dsv_buffers.Get(), "renderer_dsv_buffer");

		D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc;
		dsv_desc.Flags = D3D12_DSV_FLAG_NONE;
		dsv_desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsv_desc.Format = m_dsv_format;
		dsv_desc.Texture2D.MipSlice = 0;
		m_device->CreateDepthStencilView(m_dsv_buffers.Get(), &dsv_desc, m_dsv_cpu_handles);

		return true;
	}

	bool Renderer::ResizeViewport()
	{
		//WaitForGpuCompelete();
		FlushCommandQueue();

		if (FAILED(m_command_list->Reset(m_command_allocator.Get(), nullptr)))
		{
			LOG_ERROR("Colud not reset command list");
			return false;
		}

		for (auto& render_target : m_rtv_buffers)
			render_target.Reset();
		m_dsv_buffers.Reset();

		const auto& window = m_window.lock();

		if (FAILED(m_swap_chain->ResizeBuffers(s_swap_chain_buffer_count, window->width, window->height,
			m_rtv_format, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
		{
			LOG_ERROR("Could not resize render target");
			return false;
		}

		m_cur_swapchain_buffer = 0;

		if (CreateRenderTargetViews() == false)
		{
			LOG_ERROR("Could not create render target views");
			return false;
		}

		if (CreateDepthStencilView() == false)
		{
			LOG_ERROR("Could not create depth stencil view");
			return false;
		}

		m_command_list->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_dsv_buffers.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

		if (FAILED(m_command_list->Close()))
		{
			LOG_ERROR("Could not close command list");
			return false;
		}

		ID3D12CommandList* cmd_lists[] = { m_command_list.Get() };
		m_command_queue->ExecuteCommandLists(_countof(cmd_lists), cmd_lists);

		FlushCommandQueue();

		SetViewAndScissor(0.0f, 0.0f, static_cast<float>(window->width), static_cast<float>(window->height));

		return true;
	}

	void Renderer::SetViewAndScissor(float left, float top, float width, float height)
	{
		m_viewport.TopLeftX = left;
		m_viewport.TopLeftY = top;
		m_viewport.Width = width;
		m_viewport.Height = height;
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;

		m_scissor_rect.left = static_cast<LONG>(left);
		m_scissor_rect.top = static_cast<LONG>(top);
		m_scissor_rect.right = static_cast<LONG>(left + width);
		m_scissor_rect.bottom = static_cast<LONG>(top + height);
	}

	void Renderer::FlushCommandQueue()
	{
		++m_current_fence;

		if (FAILED(m_command_queue->Signal(m_fence.Get(), m_current_fence)))
		{
			LOG_WARN("Gpu cannot excute signal");
		}

		if (m_fence->GetCompletedValue() < m_current_fence)
		{
			HANDLE event_handle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
			if (FAILED(m_fence->SetEventOnCompletion(m_current_fence, event_handle)))
			{
				LOG_WARN("Failed to reach fence value");
			}
			WaitForSingleObject(event_handle, INFINITE);
			CloseHandle(event_handle);
		}
	}
}