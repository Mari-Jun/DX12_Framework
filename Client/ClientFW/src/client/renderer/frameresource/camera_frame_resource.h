#pragma once

namespace client_fw
{
	template<class T> class UploadBuffer;

	struct RSRenderCameraData
	{
		Mat4 view_matrix;
		Mat4 projection_matrix;
		Mat4 view_projection_matrix;
		Mat4 inverse_view_matrix;
		Vec4 perspective_values;
		Vec3 camera_position;
		UINT final_texture_index = 0;
		XMUINT4 gbuffer_texture_indices;
		UINT num_of_directional_light = 0;
		UINT render_camera_index = 0;
	};

	struct RSShadowCameraData
	{
		Mat4 view_projection_matrix;
	};


	class CameraFrameResource
	{
	public:
		CameraFrameResource();
		~CameraFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadBuffer<RSRenderCameraData>> m_render_camera_data;
		UINT m_size_of_render_camera = 1;

		UPtr<UploadBuffer<RSShadowCameraData>> m_shadow_camera_data;
		UINT m_size_of_shadow_camera = 1;

	public:
		const UPtr<UploadBuffer<RSRenderCameraData>>& GetRenderCameraData() const { return m_render_camera_data; }
		UINT GetSizeOfRenderCamera() const { return m_size_of_render_camera; }
		void SetSizeOfRenderCamera(UINT count) { m_size_of_render_camera = count; }

		const UPtr<UploadBuffer<RSShadowCameraData>>& GetShadowCameraData() const { return m_shadow_camera_data; }
		UINT GetSizeOfShadowCamera() const { return m_size_of_shadow_camera; }
		void SetSizeOfShadowCamera(UINT count) { m_size_of_shadow_camera = count; }
	};
}



