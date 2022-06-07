#pragma once

namespace client_fw
{
	struct Window;

	enum class eRenderLevelType;
	enum class eKindOfRenderLevel;
	class GraphicsSuperRootSignature;
	class GraphicsRenderLevel;
	class GraphicsShader;

	class ComputeSuperRootSignature;
	class ComputeRenderLevel;
	class ComputeShader;

	class RenderComponent;
	class SkyComponent;
	class CameraComponent;
	class RenderCameraComponent;

	class RenderResourceManager;
	class RenderCameraManager;
	class ShadowCameraManager;
	class LightComponent;
	class LightManager;

	class ImGuiSystem;

	class IngameViewport;

	class RenderSystem final
	{
	public:
		RenderSystem(const WPtr<Window>& window);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		bool Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* command_list);
		void Shutdown();

		void Update(ID3D12Device* device);
		void UpdateImGui(ID3D12Device* device);
		void UpdateInGameViewport(ID3D12Device* device);
		void PreDraw(ID3D12Device* device, ID3D12GraphicsCommandList* command_list) const;
		void Draw(ID3D12GraphicsCommandList* command_list) const;
		void DrawInGameViewport(ID3D12GraphicsCommandList* command_list) const;
		void DrawFinalView(ID3D12GraphicsCommandList* command_list) const;

		void UpdateViewport();

	public:
		template <class T>
		bool RegisterGraphicsRenderLevel(eRenderLevelType level_type)
		{
			m_graphics_render_levels[level_type] = CreateSPtr<T>(m_graphics_super_root_signature);
			return m_graphics_render_levels[level_type]->Initialize(m_device);
		}

		template <class T>
		bool RegisterGraphicsShader(const std::string& shader_name, std::vector<eRenderLevelType>&& level_types, bool is_custom = false)
		{
			bool ret = true;
			m_graphics_shaders[shader_name] = CreateSPtr<T>(shader_name);

			for (eRenderLevelType level_type : level_types)
			{
				if (m_graphics_render_levels.find(level_type) != m_graphics_render_levels.cend())
				{
					if (is_custom)
						m_added_shaders.insert(shader_name);
					m_graphics_shaders[shader_name]->AddRegisteredRenderLevelType(level_type);
					ret &= m_graphics_render_levels[level_type]->RegisterGraphicsShader(m_device, m_graphics_shaders[shader_name]);
				}
				else
				{
					LOG_WARN("Could not find {0} from render system", Render::ConvertRenderLevelType(level_type));
					return false;
				}
			}

			m_graphics_shaders[shader_name]->Initialize(m_device);

			return ret;
		}

		void UnregisterGraphicsShader(const std::string& shader_name, std::vector<eRenderLevelType>&& level_types);

		bool RegisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name);
		void UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name);
		bool RegisterSkyComponent(const SPtr<SkyComponent>& sky_comp, const std::string& shader_name);
		void UnregisterSkyComponent(const SPtr<SkyComponent>& sky_comp, const std::string& shader_name);
		bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		void SetMainCamera(const SPtr<RenderCameraComponent>& camera_comp);

	public:
		template <class T>
		bool RegisterComputeRenderLevel(eRenderLevelType level_type)
		{
			m_compute_render_levels[level_type] = CreateSPtr<T>(m_compute_super_root_signature);
			return m_compute_render_levels[level_type]->Initialize(m_device);
		}

		template <class T>
		bool RegisterComputeShader(const std::string& shader_name, std::vector<eRenderLevelType>&& level_types, bool is_custom = false)
		{
			bool ret = true;
			m_compute_shaders[shader_name] = CreateSPtr<T>(shader_name);

			for (eRenderLevelType level_type : level_types)
			{
				if (m_compute_render_levels.find(level_type) != m_compute_render_levels.cend())
				{
					if (is_custom)
						m_added_shaders.insert(shader_name);
					m_compute_shaders[shader_name]->AddRegisteredRenderLevelType(level_type);
					ret &= m_compute_render_levels[level_type]->RegisterComputeShader(m_device, m_compute_shaders[shader_name]);
				}
				else
				{
					LOG_WARN("Could not find {0} from render system", Render::ConvertRenderLevelType(level_type));
					return false;
				}
			}

			m_compute_shaders[shader_name]->Initialize(m_device);

			return ret;
		}

		void UnregisterComputeShader(const std::string& shader_name, std::vector<eRenderLevelType>&& level_types);

	private:
		WPtr<Window> m_window;
		ID3D12Device* m_device;
		bool m_wait_gpu_complete = false;

		SPtr<GraphicsSuperRootSignature> m_graphics_super_root_signature;
		std::map<eRenderLevelType, SPtr<GraphicsRenderLevel>> m_graphics_render_levels;
		std::map<std::string, SPtr<GraphicsShader>> m_graphics_shaders;

		SPtr<ComputeSuperRootSignature> m_compute_super_root_signature;
		std::map<eRenderLevelType, SPtr<ComputeRenderLevel>> m_compute_render_levels;
		std::map<std::string, SPtr<ComputeShader>> m_compute_shaders;

		std::set<std::string> m_added_shaders;

		UPtr<RenderResourceManager> m_render_asset_manager;
		UPtr<RenderCameraManager> m_render_camera_manager;
		UPtr<ShadowCameraManager> m_shadow_camera_manager;
		UPtr<LightManager> m_light_manager;

		UPtr<ImGuiSystem> m_imgui_system;

		UPtr<IngameViewport> m_ready_ingame_viewport;
		UPtr<IngameViewport> m_ingame_viewport;

	public:
		bool IsWaitGpuComplete() const { return m_wait_gpu_complete; }
		void SetWaitGpuComplete(bool value) { m_wait_gpu_complete = value; }
		void EnableIngameViewport(const IVec2& position, const IVec2& size);
		void DisableIngameViewport();
		const UPtr<IngameViewport>& GetIngameViewport() const { return m_ingame_viewport; }

	public:
		ID3D12Device* GetDevice() const { return m_device; }
		SPtr<Window> GetWindow() const { return m_window.lock(); }
	};
}
