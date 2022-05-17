#pragma once

namespace client_fw
{
	enum class eAppState
	{
		kActive,
		kPaused,
		kDead
	};

	struct Window;
	class Timer;
	class Renderer;

	class EventSystem;
	struct EventKeyInfo;
	class MessageEventInfo;
	class LevelManager;
	class LevelLoader;
	class Level;
	class LevelSharedInfo;
	class UserInterfaceManager;
	class PhysicsWorld;

	class LayerManager;
	class Layer;

	class AssetManager;
	class MeshLoader;
	class MaterialLoader;
	class TextureLoader;

	class Application
	{
	public:
		Application(std::wstring_view app_name);
		virtual ~Application();
		
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

		virtual bool Initialize();
		void InitializeAssetManager();
		virtual void Shutdown();
		void Run();

	private:
		void ExecuteEvents();
		void SendEventToServer();
		void Update(float delta_time);
		void Render();

	private:
		virtual bool InitializeWindow();
		void DestroyWindow();
		void ShowFpsToWindowTitle(UINT fps);
		void UpdateWindowSize(LONG x, LONG y);
		void UpdateWindowRect();

	protected:
		void RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);

		void OpenLevel(const SPtr<Level>& level) const;
		void OpenLevel(const SPtr<Level>& level, UPtr<LevelLoader>&& level_loader) const;
		void CloseLevel() const;

		void RegisterLayer(const SPtr<Layer>& layer) const;

	private:
		virtual SPtr<LevelSharedInfo> CreateLevelSharedInfo() const;

	protected:
		static Application* s_instance;
		eAppState m_app_state;
		std::wstring m_app_name;

		SPtr<Window> m_window;

	private:
		UPtr<Timer> m_timer;
		UPtr<EventSystem> m_event_system;
		UPtr<LevelManager> m_level_manager;
		UPtr<UserInterfaceManager> m_user_interface_manager;
		UPtr<LayerManager> m_layer_manager;

		UPtr<PhysicsWorld> m_physics_world;

		UPtr<Renderer> m_renderer;
		UPtr<AssetManager> m_asset_manager;

	public:
		inline static Application* GetApplication() { return s_instance; }
		eAppState GetAppState() { return m_app_state; }
		void SetAppState(eAppState app_state) { m_app_state = app_state; }

	private:
		virtual UPtr<MeshLoader> CreateMeshLoader() const;
		virtual UPtr<MaterialLoader> CreateMaterialLoader() const;
		virtual UPtr<TextureLoader> CreateTextureLoader() const;

	public:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};

	UPtr<Application> CreateApplication();
}


