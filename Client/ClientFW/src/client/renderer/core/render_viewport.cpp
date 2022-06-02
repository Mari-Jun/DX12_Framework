#include "stdafx.h"
#include "client/renderer/core/render_viewport.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/asset/texture/texture.h"

namespace client_fw
{
	IngameViewport::IngameViewport(const IVec2& pos, const IVec2& size)
		: m_position(pos), m_size(size)
	{
		m_viewport_texture = CreateSPtr<ViewportTexture>(size);
	}

	bool IngameViewport::Initialize(ID3D12Device* device)
	{
		RenderResourceManager::GetRenderResourceManager().RegisterTexture(m_viewport_texture);
		return true;
	}

	void IngameViewport::PreDraw(ID3D12GraphicsCommandList* command_list)
	{
		if (m_viewport_texture->GetResource() != nullptr)
		{
			D3D12_VIEWPORT view = { 0.f, 0.f, static_cast<float>(m_size.x), static_cast<float>(m_size.y), 0.0f, 1.0f };
			D3D12_RECT scissor = { 0, 0, static_cast<LONG>(m_size.x), static_cast<LONG>(m_size.y) };
			command_list->RSSetViewports(1, &view);
			command_list->RSSetScissorRects(1, &scissor);
			m_viewport_texture->PreDraw(command_list);
		}
	}

	void IngameViewport::PostDraw(ID3D12GraphicsCommandList* command_list)
	{
		if (m_viewport_texture->GetResource() != nullptr)
			m_viewport_texture->PostDraw(command_list);
	}
}
