#include "stdafx.h"
#include "client/core/window.h"
#include "client/renderer/text/text_render_system.h"
#include "client/asset/texture/texture.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/renderer/text/text_render_manager.h"
#include "client/renderer/text/text_helper.h"

namespace client_fw
{
	TextRenderSystem* TextHelper::s_render_system = nullptr;

	TextRenderSystem::TextRenderSystem()
	{
		TextHelper::s_render_system = this;
#ifdef __USE_DWRITE__
		m_text_render_manager = CreateUPtr<TextRenderManager>();
#endif
	}

	TextRenderSystem::~TextRenderSystem()
	{
	}

	bool TextRenderSystem::Initialize(ID3D12Device* device, ID3D12CommandQueue* command_queue)
	{
#ifdef __USE_DWRITE__
		if (Create11Device(device, command_queue) == false)
		{
			LOG_ERROR("Could not create DX11 device");
			return false;
		}

		if (m_text_render_manager->Initialize(m_write_factory.Get(), m_2d_device_context.Get()) == false)
		{
			LOG_ERROR("Could not initialize text render manager");
			return false;
		}

#endif
		return true;
	}

	bool TextRenderSystem::Create11Device(ID3D12Device* device, ID3D12CommandQueue* command_queue)
	{
		UINT dx11_device_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		D2D1_FACTORY_OPTIONS d2d_factory_options = {};

#if defined(_DEBUG)
		{
			dx11_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
			d2d_factory_options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
		}
#endif

		ComPtr<ID3D11Device> dx11_device;
		if (FAILED(D3D11On12CreateDevice(device, dx11_device_flags,
			nullptr, 0, reinterpret_cast<IUnknown**>(&command_queue),
			1, 0, &dx11_device, &m_dx11_device_context, nullptr)))
		{
			LOG_ERROR("Could not create 11On12 device");
			return false;
		}

		if (FAILED(dx11_device.As(&m_dx11_on_12_device)))
			return false;

		if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3),
			&d2d_factory_options, &m_2d_factory)))
		{
			LOG_ERROR("Could not create 2d factory");
			return false;
		}

		ComPtr<IDXGIDevice> dxgi_device;
		if (FAILED(m_dx11_on_12_device.As(&dxgi_device)))
			return false;
		if (FAILED(m_2d_factory->CreateDevice(dxgi_device.Get(), &m_2d_device)))
			return false;
		if (FAILED(m_2d_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_2d_device_context)))
			return false;
		if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &m_write_factory)))
			return false;

		return true;
	}

	void TextRenderSystem::Shutdown()
	{
	}

	void TextRenderSystem::Update(ID3D12Device* device)
	{
#ifdef __USE_DWRITE__
		m_text_render_manager->Update(m_write_factory.Get(), device, m_dx11_on_12_device.Get(), m_2d_device_context.Get());
#endif 
	}

	void TextRenderSystem::Draw() const
	{
#ifdef __USE_DWRITE__
		if (m_text_render_manager->Draw(m_dx11_on_12_device.Get(), m_2d_device_context.Get()))
		{
			m_dx11_device_context->Flush();
		
		}
#endif
	}
	void TextRenderSystem::PostDraw(ID3D12GraphicsCommandList* command_list) const
	{
#ifdef __USE_DWRITE__
		m_text_render_manager->PostDraw(command_list);
#endif
	}
}
