#pragma once

namespace client_fw
{
	class ViewportTexture;

	class IngameViewport
	{
	public:
		IngameViewport(const IVec2& pos, const IVec2& size);

		bool Initialize(ID3D12Device* device);

		void PreDraw(ID3D12GraphicsCommandList* command_list);
		void PostDraw(ID3D12GraphicsCommandList* command_list);

	private:
		IVec2 m_position;
		IVec2 m_size;
		SPtr<ViewportTexture> m_viewport_texture;

	public:
		const IVec2& GetViewportPosition() const { return m_position; }
		const IVec2& GetViewportSize() const { return m_size; }
		const SPtr<ViewportTexture>& GetViewportTexture() const { return m_viewport_texture; }
	};
}


