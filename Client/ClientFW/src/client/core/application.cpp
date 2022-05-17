#include "stdafx.h"
#include "client/core/application.h"
#include "client/core/window.h"
#include "client/core/timer.h"
#include "client/input/input.h"
#include "client/event/core/event_system.h"

#include "client/object/level/core/level_manager.h"
#include "client/object/level/core/level_loader.h"
#include "client/object/level/core/level.h"
#include "client/object/level/sharedinfo/level_shared_info.h"
#include "client/object/ui/core/user_interface_manager.h"
#include "client/object/layer/core/layer_manager.h"

#include "client/physics/core/physics_world.h"

#include "client/renderer/core/renderer.h"
#include "client/asset/core/asset_manager.h"
#include "client/asset/mesh/mesh_loader.h"
#include "client/asset/material/material_loader.h"
#include "client/asset/texture/texture_loader.h"

#include "client/imgui/imgui.h"

//#define __USE_RENDER_CPU_TIME__
#ifdef __USE_RENDER_CPU_TIME__
#include <stdio.h>
#include <time.h>
#endif

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace client_fw
{
	Application* Application::s_instance = nullptr;


	Application::Application(std::wstring_view app_name)
		: m_app_state(eAppState::kActive)
		, m_app_name(app_name)
	{
		s_instance = this;

		m_window = CreateSPtr<Window>(1600, 900);
		m_timer = CreateUPtr<Timer>();
		m_event_system = CreateUPtr<EventSystem>(m_window);
		m_level_manager = CreateUPtr<LevelManager>();
		m_user_interface_manager = CreateUPtr<UserInterfaceManager>();
		m_layer_manager = CreateUPtr<LayerManager>();

		m_physics_world = CreateUPtr<PhysicsWorld>();

		m_renderer = CreateUPtr<Renderer>(m_window);
		m_asset_manager = CreateUPtr<AssetManager>();
	}

	Application::~Application()
	{
	}

	bool Application::Initialize()
	{
		bool result = InitializeWindow();
		
		if (result == false)
		{
			LOG_ERROR("Could not initialize window");
			return false;
		}

		result = m_timer->Initialize();
		if (result == false)
		{
			LOG_ERROR("Could not initialize timer");
			return false;
		}
		m_timer->OnFpsChanged([this](UINT fps) {ShowFpsToWindowTitle(fps); });

		result = m_level_manager->Initiallize(CreateLevelSharedInfo());

		result = m_physics_world->Initialize();
		if (result == false)
		{
			LOG_ERROR("Could not initialize physics world");
			return false;
		}

		result = m_renderer->Initialize();
		if (result == false)
		{
			LOG_ERROR("Could not initialize renderer");
			return false;
		}

		InitializeAssetManager();

		return true;
	}

	void Application::InitializeAssetManager()
	{
		m_asset_manager->Initialize(std::move(CreateMeshLoader()),
			std::move(CreateMaterialLoader()), std::move(CreateTextureLoader()));
	}

	void Application::Shutdown()
	{
		m_level_manager->Shutdown();
		m_layer_manager->Shutdown();
		m_user_interface_manager->Shutdown();
		m_physics_world->Shutdown();
		m_renderer->Shutdown();
	}

	void Application::Run()
	{
		MSG msg;

		while (GetAppState() != eAppState::kDead)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (GetAppState() == eAppState::kActive)
			{
				ExecuteEvents();
				m_timer->Update();

				Update(m_timer->GetDeltaTime());
				SendEventToServer();

#ifdef __USE_RENDER_CPU_TIME__
				clock_t r_start, r_end;
				r_start = clock();
#endif
				Render();
#ifdef __USE_RENDER_CPU_TIME__
				r_end = clock();
				std::cout << "Render Cpu Time : " << float(r_end - r_start) << std::endl;
#endif
			}
		}
	}

	void Application::ExecuteEvents()
	{
		m_event_system->ExecuteEvent();
	}

	void Application::SendEventToServer()
	{
		m_event_system->SendEventToServer();
	}

	void Application::Update(float delta_time)
	{
		m_level_manager->Update(delta_time);
		m_physics_world->Update(delta_time);
		m_user_interface_manager->Update(delta_time);
		m_level_manager->UpdateWorldMatrix();
		if (m_renderer->Update() == false)
		{
			LOG_ERROR("Render Update Error");
			SetAppState(eAppState::kDead);
		}
		else
		{
			m_layer_manager->Update(delta_time);
		}
	}

	void Application::Render()
	{
		if (m_renderer->Render() == false)
		{
			LOG_ERROR("Rendering Error");
			SetAppState(eAppState::kDead);
		}
	}

	void Application::UpdateWindowSize(LONG x, LONG y)
	{
		UpdateWindowRect();
		m_window->width = x;
		m_window->height = y;
		m_window->mid_pos.x = m_window->width / 2;
		m_window->mid_pos.y = m_window->height / 2;

		//현재 사이즈를 키우려면 카메라, UI등의 텍스쳐도 재 생성이 필요하다.
		if (m_renderer->UpdateViewport() == false)
		{
			SetAppState(eAppState::kDead);
		}
	}

	void Application::UpdateWindowRect()
	{
		GetWindowRect(m_window->hWnd, &m_window->rect);	
	}

	void Application::RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
		const std::function<bool()>& func, bool consumption)
	{
		Input::RegisterPressedEvent(name, std::move(keys), func, consumption, eInputOwnerType::kApplication);
	}

	void Application::OpenLevel(const SPtr<Level>& level) const
	{
		m_level_manager->OpenLevel(level, nullptr);
	}

	void Application::OpenLevel(const SPtr<Level>& level, UPtr<LevelLoader>&& level_loader) const
	{
		m_level_manager->OpenLevel(level, std::move(level_loader));
	}

	void Application::CloseLevel() const
	{
		m_level_manager->CloseLevel();
	}

	void Application::RegisterLayer(const SPtr<Layer>& layer) const
	{
		m_layer_manager->RegisterLayer(layer);
	}

	SPtr<LevelSharedInfo> Application::CreateLevelSharedInfo() const
	{
		return CreateSPtr<LevelSharedInfo>();
	}

	bool Application::InitializeWindow()
	{
		m_window->hInstance = GetModuleHandle(nullptr);
		if (m_window->hInstance == nullptr)
			return false;

		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_window->hInstance;
		wcex.hIcon = LoadIcon(m_window->hInstance, IDI_WINLOGO);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = m_app_name.c_str();
		wcex.hIconSm = wcex.hIcon;

		RegisterClassExW(&wcex);

		int posX = (GetSystemMetrics(SM_CXSCREEN) == m_window->width) ? 0 : (GetSystemMetrics(SM_CXSCREEN) - m_window->width) / 2;
		int posY = (GetSystemMetrics(SM_CYSCREEN) == m_window->height) ? 0 : (GetSystemMetrics(SM_CYSCREEN) - m_window->height) / 2;

		DWORD dw_style = WS_OVERLAPPED | WS_MINIMIZEBOX | /*WS_SYSMENU |*/ WS_BORDER;

		m_window->hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_app_name.c_str(), m_app_name.c_str(),
			/*WS_OVERLAPPEDWINDOW*/dw_style, posX, posY, m_window->width, m_window->height, NULL, NULL, m_window->hInstance, NULL);

		if (m_window->hWnd == nullptr)
			return false;
		
		//SetWindowLong(m_window->hWnd, GWL_STYLE, 0);

#ifdef _DEBUG
		ShowWindow(GetConsoleWindow(), SW_SHOW);
#else
		ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif 
		ShowWindow(m_window->hWnd, SW_SHOW);
		SetForegroundWindow(m_window->hWnd);
		SetFocus(m_window->hWnd);
		return true;
	}

	void Application::DestroyWindow()
	{
		::DestroyWindow(m_window->hWnd);
		UnregisterClassW(m_app_name.c_str(), m_window->hInstance);
	}

	void Application::ShowFpsToWindowTitle(UINT fps)
	{
		std::wstring title = m_app_name;
		title += L" (FPS : " + std::to_wstring(fps) + L")";
		SetWindowTextW(m_window->hWnd, title.c_str());
	}

	UPtr<MeshLoader> Application::CreateMeshLoader() const
	{
		return CreateUPtr<MeshLoader>();
	}

	UPtr<MaterialLoader> Application::CreateMaterialLoader() const
	{
		return CreateUPtr<MaterialLoader>();
	}

	UPtr<TextureLoader> Application::CreateTextureLoader() const
	{
		return CreateUPtr<TextureLoader>();
	}

	LRESULT CALLBACK Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (Input::GetInputMode() == eInputMode::kUIAndGame || Input::GetInputMode() == eInputMode::kUIOnly)
		{
			if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
				return true;
		}

		LRESULT result = NULL;

		const auto& app = Application::GetApplication();
		const auto& input_system = app->m_event_system;

		switch (message)
		{
		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED)
			{
				app->SetAppState(client_fw::eAppState::kPaused);
			}
			else if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
			{
				app->SetAppState(client_fw::eAppState::kActive);
				app->UpdateWindowSize(LOWORD(lParam), HIWORD(lParam));
			}
			break;
		case WM_ACTIVATE:
		{
			static eInputMode s_input_mode = eInputMode::kUIOnly;
			switch (wParam)
			{
			case WA_ACTIVE:
				Input::SetOnlyInputMode(s_input_mode);
				break;
			case WA_CLICKACTIVE:
				Input::SetInputMode(s_input_mode);
				break;
			case WA_INACTIVE:
				s_input_mode = Input::GetInputMode();
				Input::SetInputMode(eInputMode::kInActive);
				break;
			}
			break;
		}
		case WM_MOVE:
			app->UpdateWindowRect();
			break;
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		default:
			result = DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}

		input_system->ChangeInputState(hWnd, message, wParam, lParam);

		return result;
	}

}