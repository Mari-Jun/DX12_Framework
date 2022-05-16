#pragma once

namespace client_fw
{
	//수정 하지 마세요.
	constexpr static UINT s_max_cascade_level = 3;

	class CameraComponent;
	class RenderCameraComponent;
	class ShadowCameraComponent;
	class ShadowCubeCameraComponent;
	class ShadowCascadeCameraComponent;

	class ShadowCameraManager final
	{
	public:
		ShadowCameraManager();
		~ShadowCameraManager();

		ShadowCameraManager(const ShadowCameraManager&) = delete;
		ShadowCameraManager& operator=(const ShadowCameraManager&) = delete;

		void Update(ID3D12Device* device, 
			std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_camera,
			std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_cube_camera,
			std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_cascade_camera);

		void Draw(ID3D12GraphicsCommandList* command_list,
			std::function<void(ID3D12GraphicsCommandList*)>&& shadow_function,
			std::function<void(ID3D12GraphicsCommandList*)>&& shadow_cube_function,
			std::function<void(ID3D12GraphicsCommandList*)>&& shadow_cascade_function);

		bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);

	private:
		void UpdateShadowCameras(ID3D12Device* device);
		void UpdateShadowCubeCameras(ID3D12Device* device);
		void UpdateShadowCascadeCameras(ID3D12Device* device);

		void UpdateCameraResource(ID3D12Device* device,
			std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_camera,
			std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_cube_camera,
			std::function<void(ID3D12Device*)>&& update_shader_function_for_shadow_cascade_camera);

		template <class T>
		void UnregisterCameraComponent(std::vector<SPtr<T>>& cameras, const SPtr<CameraComponent>& camera_comp)
		{
			auto iter = std::find(cameras.begin(), cameras.end(), camera_comp);
			if (iter != cameras.end())
			{
				std::iter_swap(iter, cameras.end() - 1);
				cameras.pop_back();
			}
		}

	private:
		std::vector<SPtr<ShadowCameraComponent>> m_ready_shadow_cameras;
		std::vector<SPtr<ShadowCameraComponent>> m_wait_resource_shadow_cameras;
		std::vector<SPtr<ShadowCameraComponent>> m_shadow_cameras;
		std::vector<SPtr<ShadowCameraComponent>> m_need_write_static_depth_shadow_cameras;

		std::vector<SPtr<ShadowCubeCameraComponent>> m_ready_shadow_cube_cameras;
		std::vector<SPtr<ShadowCubeCameraComponent>> m_wait_resource_shadow_cube_cameras;
		std::vector<SPtr<ShadowCubeCameraComponent>> m_shadow_cube_cameras;
		std::vector<SPtr<ShadowCubeCameraComponent>> m_need_write_static_depth_shadow_cube_cameras;

		std::vector<SPtr<ShadowCascadeCameraComponent>> m_ready_shadow_cascade_cameras;
		std::vector<SPtr<ShadowCascadeCameraComponent>> m_wait_resource_shadow_cascade_cameras;
		std::vector<SPtr<ShadowCascadeCameraComponent>> m_shadow_cascade_cameras;
	};
}


