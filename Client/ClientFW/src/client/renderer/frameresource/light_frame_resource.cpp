#include "stdafx.h"
#include "client/renderer/frameresource/light_frame_resource.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	LightFrameResource::LightFrameResource()
	{
		m_light_data = CreateUPtr<UploadBuffer<RSLightData>>();
	}

	LightFrameResource::~LightFrameResource()
	{
	}

	bool LightFrameResource::Initialize(ID3D12Device* device)
	{
		m_light_data->CreateResource(device, m_size_of_light);
		return true;
	}

	void LightFrameResource::Shutdown()
	{
		m_light_data->Shutdown();
	}

	LocalLightFrameResource::LocalLightFrameResource()
	{
		m_light_instance_data = CreateUPtr<UploadBuffer<RSLocalLightInstanceData>>(false);
	}

	LocalLightFrameResource::~LocalLightFrameResource()
	{
	}

	bool LocalLightFrameResource::Initialize(ID3D12Device* device)
	{
		return true;
	}

	void LocalLightFrameResource::Shutdown()
	{
		m_light_instance_data->Shutdown();
	}

	LocalLightInstanceDrawInfo LocalLightFrameResource::GetLocalLightDrawInfo()
	{
		LocalLightInstanceDrawInfo info = m_local_light_draw_info.front();
		m_local_light_draw_info.pop();
		return info;
	}
}
