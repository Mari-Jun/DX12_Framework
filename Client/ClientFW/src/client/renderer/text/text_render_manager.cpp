#include "stdafx.h"
#include "client/renderer/text/text_render_manager.h"
#include "client/renderer/text/text_helper.h"
#include "client/renderer/core/render_resource_manager.h"
#include "client/asset/texture/texture.h"

namespace client_fw
{
	TextRenderManager::TextRenderManager()
	{
	}

	TextRenderManager::~TextRenderManager()
	{
	}

	bool TextRenderManager::Initialize(IDWriteFactory* factory, ID2D1DeviceContext2* context)
	{
		return true;
	}

	void TextRenderManager::Shutdown()
	{
	}

	void TextRenderManager::Update(IDWriteFactory* factory, ID3D12Device* device_12, ID3D11On12Device* device_11, ID2D1DeviceContext2* context)
	{
		for (const auto& text : m_visit_texts)
		{
			if (text->GetTextTexture()->Initialize(device_12) == false ||
				text->GetTextTexture()->Initialize2D(device_11, context) == false)
				LOG_ERROR(L"Could not create render ui texture : info[{0}]", text->GetText());
			else
			{
				RenderResourceManager::GetRenderResourceManager().RegisterTexture(text->GetTextTexture());
			}

			LoadTextFormat(factory, text->GetTextFormat());
			LoadTextBrush(context, text->GetColor());
		}

		for (const auto& text : m_revisit_texts)
		{
			LoadTextFormat(factory, text->GetTextFormat());
			LoadTextBrush(context, text->GetColor());
		}
	}

	bool TextRenderManager::Draw(ID3D11On12Device* device, ID2D1DeviceContext2* context) const
	{
		if (m_updating_texts.empty())
			return false;

		std::vector<ID3D11Resource*> resources;
		resources.reserve(m_updating_texts.size());

		for (const auto& text : m_updating_texts)
			resources.push_back(text->GetTextTexture()->GetWrappedRenderTarget().Get());

		device->AcquireWrappedResources(resources.data(), static_cast<UINT>(resources.size()));

		for (const auto& text : m_updating_texts)
		{
			context->SetTarget(text->GetTextTexture()->Get2DRenderTarget());
			context->BeginDraw();
			context->Clear();

			D3D12_RESOURCE_DESC desc = text->GetTextTexture()->GetResource()->GetDesc();

			D2D1_RECT_F textRect = D2D1::RectF(0.f, 0.f, static_cast<float>(desc.Width), static_cast<float>(desc.Height));

			context->DrawText(
				text->GetText().c_str(),
				static_cast<UINT32>(text->GetText().size()),
				m_text_formats.at(text->GetTextFormat()).Get(),
				&textRect,
				m_text_brushes.at(text->GetColor()).Get()
			);

			context->EndDraw();
		}

		device->ReleaseWrappedResources(resources.data(), static_cast<UINT>(resources.size()));

		return true;
	}

	void TextRenderManager::PostDraw(ID3D12GraphicsCommandList* command_list)
	{
		m_updating_texts.clear();

		if (m_revisit_texts.empty() == false)
		{
			std::vector<D3D12_RESOURCE_BARRIER> barriers;
			barriers.reserve(m_revisit_texts.size());

			for (const auto& text : m_revisit_texts)
			{
				barriers.push_back(CD3DX12_RESOURCE_BARRIER::Transition(text->GetTextTexture()->GetResource(),
					D3D12_RESOURCE_STATE_GENERIC_READ, D3D12_RESOURCE_STATE_RENDER_TARGET));
				m_updating_texts.push_back(text);
			}

			m_revisit_texts.clear();

			command_list->ResourceBarrier(static_cast<UINT>(barriers.size()), barriers.data());
		}
		
		for (const auto& text : m_visit_texts)
			m_updating_texts.push_back(text);

		m_visit_texts.clear();
	}

	ComPtr<ID2D1SolidColorBrush> TextRenderManager::LoadTextBrush(ID2D1DeviceContext2* context, const BrushColor& color)
	{

		ComPtr<ID2D1SolidColorBrush> brush;
		auto iter = m_text_brushes.find(BrushColor(color));
		if (iter != m_text_brushes.cend())
			return iter->second.Get();
		else
		{
			const auto& brush_color = color.GetColor();
			if (FAILED((context->CreateSolidColorBrush(D2D1::ColorF(brush_color.x, brush_color.y, brush_color.z, brush_color.w), &brush))))
			{
				LOG_ERROR("Could not create solid brush [{0}]", color.GetColor());
				return false;
			}
			else
			{
				m_text_brushes[color] = brush;
				return brush;
			}
		}
	}

	ComPtr<IDWriteTextFormat> TextRenderManager::LoadTextFormat(IDWriteFactory* factory, const TextFormat& format)
	{
		ComPtr<IDWriteTextFormat> text_format;
		auto iter = m_text_formats.find(format);
		if (iter != m_text_formats.cend())
			return iter->second.Get();
		else
		{
			if (FAILED(factory->CreateTextFormat(format.GetFontName().c_str(), NULL,
				format.GetFontWeight(), format.GetFontStyle(), DWRITE_FONT_STRETCH_NORMAL,
				static_cast<FLOAT>(format.GetFontSize()), L"ko-kr", &text_format)))
			{
				LOG_ERROR(L"Could not create text format [{0}, {1}]", format.GetFontName(), format.GetFontSize());
				return false;
			}
			else
			{
				m_text_formats[format] = text_format;
				text_format->SetTextAlignment(format.GetTextAlignment());
				text_format->SetParagraphAlignment(format.GetParagraphAlignment());
				return text_format;
			}
		}
	}

	void TextRenderManager::RegisterText(const SPtr<TextInfo>& info)
	{
		if (info->GetTextTexture()->GetResource() == nullptr)
			m_visit_texts.push_back(info);
		else
			m_revisit_texts.push_back(info);
	}
}
