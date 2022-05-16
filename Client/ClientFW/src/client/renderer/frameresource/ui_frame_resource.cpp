#include "stdafx.h"
#include "client/renderer/frameresource/ui_frame_resource.h"
#include "client/asset/primitive/primitive.h"

namespace client_fw
{
	UserInterfaceFrameResource::UserInterfaceFrameResource()
	{
		m_ui_primitive = CreateUPtr<UploadPrimitive<UIVertex>>();
	}

	UserInterfaceFrameResource::~UserInterfaceFrameResource()
	{
	}

	bool UserInterfaceFrameResource::Initialize(ID3D12Device* device)
	{
		return m_ui_primitive->Initialize(device);
	}

	void UserInterfaceFrameResource::Shutdown()
	{
		m_ui_primitive->Shutdown();
	}
}
