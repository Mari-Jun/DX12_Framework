#pragma once

namespace client_fw
{
	static constexpr int s_max_blur_radius = 10;

	template<class T> class UploadBuffer;

	struct RSRenderCameraPostProcessingData
	{
		int blur_radius = 5;
		Vec3 paading_0;
		std::array<float, s_max_blur_radius * 2 + 1> blur_weights;
		Vec3 padding_1;
	};

	class RenderCameraPostProcessingFrameResource
	{
	public:
		RenderCameraPostProcessingFrameResource();
		~RenderCameraPostProcessingFrameResource();

		RenderCameraPostProcessingFrameResource(const RenderCameraPostProcessingFrameResource&) = delete;
		RenderCameraPostProcessingFrameResource& operator=(const RenderCameraPostProcessingFrameResource&) = delete;

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadBuffer<RSRenderCameraPostProcessingData>> m_post_processing_data;
		UINT m_size_of_post_processing = 1;

	public:
		const UPtr<UploadBuffer<RSRenderCameraPostProcessingData>>& GetRenderCameraPostProcessingData() const { return m_post_processing_data; }
		UINT GetSizeOfPostProcessing() const { return m_size_of_post_processing; }
		void SetSizeOfPostProcessing(UINT count) { m_size_of_post_processing = count; }

	};
}


