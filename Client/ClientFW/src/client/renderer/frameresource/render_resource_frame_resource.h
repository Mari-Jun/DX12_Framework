#pragma once

namespace client_fw
{
	template<class T> class UploadBuffer;

	struct RSMaterialData
	{
		Vec4 base_color;
		FLOAT roughness;
		FLOAT metallic;
		INT diffuse_texture_index;
		INT normal_texture_index;
		INT roughness_texture_index;
		INT metalic_texture_index;
	};

	class RenderResourceFrameResource
	{
	public:
		RenderResourceFrameResource();
		~RenderResourceFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadBuffer<RSMaterialData>> m_material_data;
		UINT m_size_of_material_data = 1;
		UINT m_num_of_material = 0;

	public:
		const UPtr<UploadBuffer<RSMaterialData>>& GetMaterialData() const { return m_material_data; }
		UINT GetSizeOfMaterialData() const { return m_size_of_material_data; }
		void SetSizeOfMaterialData(UINT count) { m_size_of_material_data = count; }
		UINT GetNumOfMaterial() const { return m_num_of_material; }
		void SetNumOfMaterial(UINT count) { m_num_of_material = count; }
	};

}


