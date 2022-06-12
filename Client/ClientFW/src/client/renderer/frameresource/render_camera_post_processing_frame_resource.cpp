#include "stdafx.h"
#include "client/renderer/frameresource/render_camera_post_processing_frame_resource.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	RenderCameraPostProcessingFrameResource::RenderCameraPostProcessingFrameResource()
	{
		m_post_processing_data = CreateUPtr<UploadBuffer<RSRenderCameraPostProcessingData>>(true);
	}

	RenderCameraPostProcessingFrameResource::~RenderCameraPostProcessingFrameResource()
	{
	}

	bool RenderCameraPostProcessingFrameResource::Initialize(ID3D12Device* device)
	{
		m_post_processing_data->CreateResource(device, m_size_of_post_processing);
		return true;
	}

	void RenderCameraPostProcessingFrameResource::Shutdown()
	{
		m_post_processing_data->Shutdown();
	}
}
