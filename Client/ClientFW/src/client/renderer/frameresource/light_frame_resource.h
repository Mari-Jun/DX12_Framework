#pragma once

namespace client_fw
{
	template<class T> class UploadBuffer;

	struct RSLightData
	{
		Vec3 light_color;
		float attenuation_radius = 0.0f;
		Vec3 light_direction;
		float cone_inner_angle = 0.0f;
		Vec3 light_position;
		float cone_outer_angle = 0.0f;
		INT use_shadow = 0;
		INT is_static_light = 0;
		UINT shadow_texture_data_index = 0;
	};

	class LightFrameResource
	{
	public:
		LightFrameResource();
		~LightFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadBuffer<RSLightData>> m_light_data;
		UINT m_size_of_light = 1;

	public:
		const UPtr<UploadBuffer<RSLightData>>& GetLightData() const { return m_light_data; }
		UINT GetSizeOfLight() const { return m_size_of_light; }
		void SetSizeOfLight(UINT count) { m_size_of_light = count; }
	};

	struct RSLocalLightInstanceData
	{
		Mat4 world_matrix;
		Mat4 world_inverse_transpose;
		UINT m_light_index;
	};

	//
	// 카메라마다 저장되는 Light들의 정보
	// 예를 들어 카메라 1에서 Light를 3개,
	// 카메라 2에서 Light를 5개 그린다고 하면
	// 저장은 {0, 3}, {3, 5} 방식으로 저장된다.
	//
	struct LocalLightInstanceDrawInfo
	{
		UINT start_index;
		UINT num_of_instance_data;
	};

	class LocalLightFrameResource
	{
	public:
		LocalLightFrameResource();
		~LocalLightFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadBuffer<RSLocalLightInstanceData>> m_light_instance_data;

		UINT m_size_of_light_data = 1;

		std::queue<LocalLightInstanceDrawInfo> m_local_light_draw_info;

	public:
		const UPtr<UploadBuffer<RSLocalLightInstanceData>>& GetLightInstanceData() const { return m_light_instance_data; }

		UINT GetSizeOfLightInstanceData() const { return m_size_of_light_data; }
		void SetSizeOfLightInstanceData(UINT value) { m_size_of_light_data = value; }

		void AddLocalLightDrawInfo(LocalLightInstanceDrawInfo&& info) { m_local_light_draw_info.emplace(std::move(info)); }

		LocalLightInstanceDrawInfo GetLocalLightDrawInfo();
	};
}



