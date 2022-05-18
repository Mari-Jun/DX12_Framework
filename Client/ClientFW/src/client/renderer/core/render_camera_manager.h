#pragma once

namespace client_fw
{
	

	enum class eCameraUsage;
	class CameraComponent;
	class RenderCameraComponent;
	class ShadowCameraComponent;
	class ShadowCubeCameraComponent;
	class ShadowCascadeCameraComponent;

	// 카메라의 정보를 GPU에서 사용하기 위해 Camera를 관리하는 클래스
	class RenderCameraManager final
	{
	public:
		RenderCameraManager();
		~RenderCameraManager();

		RenderCameraManager(const RenderCameraManager&) = delete;
		RenderCameraManager& operator=(const RenderCameraManager&) = delete;

		void Shutdown();
		void Update(ID3D12Device* device,
			std::function<void(ID3D12Device*)>&& update_shader_function_for_render_camera);
		void UpdateMainCameraViewport(LONG width, LONG height);

		void Draw(ID3D12GraphicsCommandList* command_list, 
			std::function<void(ID3D12GraphicsCommandList*)>&& before_deferred_function,
			std::function<void(ID3D12GraphicsCommandList*)>&& deferred_function,
			std::function<void(ID3D12GraphicsCommandList*)>&& after_deferred_function);

		void DrawMainCameraForUI(ID3D12GraphicsCommandList* command_list);

		bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);

	private:
		void ResizeRenderCamerasTexture(ID3D12Device* device);
		void UpdateRenderCameras(ID3D12Device* device);
		void UpdateRenderCamerasForCascadeShadow(ID3D12Device* device);
		void UpdateCameraResource(ID3D12Device* device, 
			std::function<void(ID3D12Device*)>&& update_shader_function_for_render_camera);

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

		void UnregisterRenderCameraToDirectionalLights(const SPtr<RenderCameraComponent>& camera_comp);

	private:
		std::vector<SPtr<RenderCameraComponent>> m_ready_render_cameras;
		std::vector<SPtr<RenderCameraComponent>> m_wait_resource_render_cameras;
		std::vector<SPtr<RenderCameraComponent>> m_render_cameras;

		SPtr<RenderCameraComponent> m_ready_main_camera;
		SPtr<RenderCameraComponent> m_main_camera;

	public:
		const SPtr<RenderCameraComponent> GetMainCamera() { return m_main_camera; }
		void SetMainCamera(const SPtr<RenderCameraComponent>& camera_comp) { m_ready_main_camera = camera_comp; }
	};
}



