#pragma once

namespace client_fw
{
	template<class T> class UploadBuffer;

	struct RSShadowTextureData
	{
		Mat4 uv_from_ndc_matrix;
		Vec2 inverse_texture_size;
		UINT shadow_texture_index = 0;
	};

	struct RSCascadeShadowTextureData
	{
		Vec4 cascade_offset_x;
		Vec4 cascade_offset_y;
		Vec4 cascade_scale;
	};

	class ShadowFrameResource
	{
	public:
		ShadowFrameResource();
		~ShadowFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadBuffer<RSShadowTextureData>> m_shadow_texture_data;
		UINT m_size_of_shadow_texture = 1;

		UPtr<UploadBuffer<RSCascadeShadowTextureData>> m_cascade_shadow_texture_data;
		UINT m_size_of_cascade_shadow_texture = 1;

	public:
		const UPtr<UploadBuffer<RSShadowTextureData>>& GetShadowTextureData() const { return m_shadow_texture_data; }
		UINT GetSizeOfShadowTexture() const { return m_size_of_shadow_texture; }
		void SetSizeOfShadowTexture(UINT count) { m_size_of_shadow_texture = count; }

		const UPtr<UploadBuffer<RSCascadeShadowTextureData>>& GetCascadeShadowTextureData() const { return m_cascade_shadow_texture_data; }
		UINT GetSizeOfCascadeShadowTexture() const { return m_size_of_cascade_shadow_texture; }
		void SetSizeOfCascadeShadowTexture(UINT count) { m_size_of_cascade_shadow_texture = count; }
	};
}



