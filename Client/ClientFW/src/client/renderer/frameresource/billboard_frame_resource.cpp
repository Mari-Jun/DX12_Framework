#include "stdafx.h"
#include "client/renderer/frameresource/billboard_frame_resource.h"
#include "client/asset/primitive/primitive.h"

namespace client_fw
{
	BillboardFrameResource::BillboardFrameResource()
	{
		m_billboard_primitive = CreateUPtr<UploadPrimitive<BillboardVertex>>();
	}
	BillboardFrameResource::~BillboardFrameResource()
	{
	}

	bool BillboardFrameResource::Initialize(ID3D12Device* device)
	{
		bool ret = true;
		ret &= m_billboard_primitive->Initialize(device);
		return ret;
	}

	void BillboardFrameResource::Shutdown()
	{
		m_billboard_primitive->Shutdown();
	}

	BillboardDrawInfo BillboardFrameResource::GetBillboardDrawInfo()
	{
		BillboardDrawInfo info = m_draw_info.front();
		m_draw_info.pop();
		return info;
	}
}
