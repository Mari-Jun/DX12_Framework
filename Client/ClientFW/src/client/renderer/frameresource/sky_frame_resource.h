#pragma once

namespace client_fw
{
	template<class T> class UploadBuffer;
	template<class VertexType> class UploadPrimitive;
	class SkySphereVertex;

	struct RSCubeTextureIndexData
	{
		UINT index;
	};

	class SkyFrameResource
	{
	public:
		SkyFrameResource();
		~SkyFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadBuffer<RSCubeTextureIndexData>> m_cube_texture_index_data;
		UPtr<UploadPrimitive<SkySphereVertex>> m_sky_sphere_primitive;

	public:
		const UPtr<UploadBuffer<RSCubeTextureIndexData>>& GetCubeTextureIndexData() const { return m_cube_texture_index_data; }
		const UPtr<UploadPrimitive<SkySphereVertex>>& GetSkySpherePrimitive() const { return m_sky_sphere_primitive; }
	};
}



