#include "stdafx.h"
#include "client/renderer/frameresource/sky_frame_resource.h"
#include "client/asset/primitive/primitive.h"
#include "client/util/upload_buffer.h"

namespace client_fw
{
	SkyFrameResource::SkyFrameResource()
	{
		m_cube_texture_index_data = CreateUPtr<UploadBuffer<RSCubeTextureIndexData>>(true);
		m_sky_sphere_primitive = CreateUPtr<UploadPrimitive<SkySphereVertex>>();
	}

	SkyFrameResource::~SkyFrameResource()
	{
	}

	bool SkyFrameResource::Initialize(ID3D12Device* device)
	{
		m_cube_texture_index_data->CreateResource(device, 1);
		bool ret = m_sky_sphere_primitive->Initialize(device);
		m_sky_sphere_primitive->Update(device, 2);
		m_sky_sphere_primitive->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);
		return true;
	}

	void SkyFrameResource::Shutdown()
	{
		m_sky_sphere_primitive->Shutdown();
		m_cube_texture_index_data->Shutdown();
	}
}