#include "stdafx.h"
#include "client/renderer/renderlevel/core/render_level.h"
#include "client/renderer/shader/core/shader.h"
#include "client/renderer/shader/core/graphics_shader.h"
#include "client/renderer/rootsignature/graphics_super_root_signature.h"

namespace client_fw
{
	RenderLevel::RenderLevel(eRenderLevelType level_type)
		: m_level_type(level_type)
	{
	}

	GraphicsRenderLevel::GraphicsRenderLevel(eRenderLevelType level_type,
		const SPtr<GraphicsSuperRootSignature>& root_signature,
		std::vector<DXGI_FORMAT>&& rtv_formats, DXGI_FORMAT dsv_format)
		: RenderLevel(level_type), m_graphics_root_signature(root_signature)
		, m_rtv_formats(rtv_formats), m_dsv_format(dsv_format)
	{
	}

	void GraphicsRenderLevel::Shutdown()
	{
		m_graphics_shaders.clear();
	}

	void GraphicsRenderLevel::Update(ID3D12Device* device)
	{
		for (const auto& shader : m_graphics_shaders)
			shader->Update(device, m_level_type);
	}

	void GraphicsRenderLevel::UpdateFrameResource(ID3D12Device* device)
	{
		for (const auto& shader : m_graphics_shaders)
			shader->UpdateFrameResource(device, m_level_type);
	}

	void GraphicsRenderLevel::Draw(ID3D12GraphicsCommandList* command_list) const
	{
		for (const auto& shader : m_graphics_shaders)
			shader->Draw(command_list, m_level_type);
	}

	bool GraphicsRenderLevel::RegisterGraphicsShader(ID3D12Device* device, const SPtr<GraphicsShader>& graphics_shader)
	{
		m_graphics_shaders.push_back(graphics_shader);
		return graphics_shader->CreatePipelineStates(device, shared_from_this());
	}

	void GraphicsRenderLevel::UnregisterGraphicsShader(const SPtr<GraphicsShader>& graphics_shader)
	{
		auto iter = std::find(m_graphics_shaders.begin(), m_graphics_shaders.end(), graphics_shader);
		if (iter != m_graphics_shaders.cend())
		{
			std::iter_swap(iter, m_graphics_shaders.end() - 1);
			m_graphics_shaders.pop_back();
		}
	}
}