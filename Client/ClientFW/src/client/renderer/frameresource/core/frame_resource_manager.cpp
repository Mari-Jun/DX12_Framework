#include "stdafx.h"
#include "client/renderer/frameresource/core/frame_resource_manager.h"
#include "client/renderer/frameresource/core/frame_resource.h"

namespace client_fw
{
	FrameResourceManager* FrameResourceManager::s_resource_manager = nullptr;

	FrameResourceManager::FrameResourceManager()
	{
		s_resource_manager = this;

		for (UINT index = 0; index < s_num_of_frame_resource; ++index)
			m_frame_resources[index] = CreateUPtr<FrameResource>();
	}

	FrameResourceManager::~FrameResourceManager()
	{
		s_resource_manager = nullptr;
	}

	bool FrameResourceManager::Initialize(ID3D12Device* device)
	{
		bool ret = true;
		for (const auto& frame_resource : m_frame_resources)
			ret &= frame_resource->Initialize(device);
		return ret;
	}

	void FrameResourceManager::Shutdown()
	{
		for (const auto& frame_resource : m_frame_resources)
			frame_resource->Shutdown();
	}
}