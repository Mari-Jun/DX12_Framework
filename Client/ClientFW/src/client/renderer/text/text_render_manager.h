#pragma once

namespace client_fw
{
	class BrushColor;
	class TextInfo;
	class TextFormat;

	class TextRenderManager final
	{
	public:
		TextRenderManager();
		~TextRenderManager();

		TextRenderManager(const TextRenderManager&) = delete;
		TextRenderManager& operator=(const TextRenderManager&) = delete;

		bool Initialize(IDWriteFactory* factory, ID2D1DeviceContext2* context);
		void Shutdown();

		void Update(IDWriteFactory* factory, ID3D12Device* device_12, ID3D11On12Device* device_11, ID2D1DeviceContext2* context);
		bool Draw(ID3D11On12Device* device, ID2D1DeviceContext2* context) const;
		void PostDraw(ID3D12GraphicsCommandList* command_list);

	private:
		std::map<BrushColor, ComPtr<ID2D1SolidColorBrush>> m_text_brushes;
		std::map<TextFormat, ComPtr<IDWriteTextFormat>> m_text_formats;

		std::vector<SPtr<TextInfo>> m_visit_texts;
		std::vector<SPtr<TextInfo>> m_revisit_texts;
		std::vector<SPtr<TextInfo>> m_updating_texts;

	public:
		ComPtr<ID2D1SolidColorBrush> LoadTextBrush(ID2D1DeviceContext2* context, const BrushColor& color);
		ComPtr<IDWriteTextFormat> LoadTextFormat(IDWriteFactory* factory, const TextFormat& format);

		void RegisterText(const SPtr<TextInfo>& info);
	};
}


