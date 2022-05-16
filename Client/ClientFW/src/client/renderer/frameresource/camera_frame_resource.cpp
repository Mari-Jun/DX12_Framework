#include "stdafx.h"
#include "client/renderer/frameresource/camera_frame_resource.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	CameraFrameResource::CameraFrameResource()
	{
		m_render_camera_data = CreateUPtr<UploadBuffer<RSRenderCameraData>>(true);
		m_shadow_camera_data = CreateUPtr<UploadBuffer<RSShadowCameraData>>();
	}

	CameraFrameResource::~CameraFrameResource()
	{
	}

	bool CameraFrameResource::Initialize(ID3D12Device* device)
	{
		m_render_camera_data->CreateResource(device, m_size_of_render_camera);
		m_shadow_camera_data->CreateResource(device, m_size_of_shadow_camera);
		return true;
	}

	void CameraFrameResource::Shutdown()
	{
		m_shadow_camera_data->Shutdown();
		m_render_camera_data->Shutdown();
	}
}
