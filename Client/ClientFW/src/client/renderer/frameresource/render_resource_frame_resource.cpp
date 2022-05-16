#include "stdafx.h"
#include "client/renderer/frameresource/render_resource_frame_resource.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	RenderResourceFrameResource::RenderResourceFrameResource()
	{
		m_material_data = CreateUPtr<UploadBuffer<RSMaterialData>>();
	}

	RenderResourceFrameResource::~RenderResourceFrameResource()
	{
	}

	bool RenderResourceFrameResource::Initialize(ID3D12Device* device)
	{
		return true;
	}

	void RenderResourceFrameResource::Shutdown()
	{
		m_material_data->Shutdown();
	}
}
