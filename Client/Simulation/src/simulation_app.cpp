#include <include/client_fw.h>
#include <client/core/entry_point.h>

#include "object/level/rotating_cube_level.h"

#include "object/layer/imgui_demo_layer.h"
#include "object/layer/docking_layer.h"
#include "object/layer/viewport_layer.h"
#include "object/layer/select_level_layer.h"
#include "object/layer/level_setting_layer.h"
#include "object/layer/menu_layer.h"

using namespace client_fw;

namespace simulation
{
	class SimulationApp : public client_fw::Application
	{
	public:
		SimulationApp() : Application(L"Simulation App")
		{
			m_viewport_layer = CreateSPtr<ViewportLayer>();
			m_menu_layer = CreateSPtr<MenuLayer>();
			m_select_level_layer = CreateSPtr<SelectLevelLayer>();
			m_level_setting_layer = CreateSPtr<LevelSettingLayer>();
		}

		bool Initialize() override
		{
			bool result = Application::Initialize();

			RegisterPressedEvent("Input Mode Game Only", std::vector{ EventKeyInfo{eKey::k1, {eAdditionalKey::kControl, eAdditionalKey::kShift}} },
				[]()->bool { Input::SetInputMode(eInputMode::kGameOnly); return true;  });
			RegisterPressedEvent("Input Mode Game And UI", std::vector{ EventKeyInfo{eKey::k2, {eAdditionalKey::kControl, eAdditionalKey::kShift}} },
				[]()->bool { Input::SetInputMode(eInputMode::kUIAndGame); return true;  });
			RegisterPressedEvent("Input Mode UI Only", std::vector{ EventKeyInfo{eKey::k3, {eAdditionalKey::kControl, eAdditionalKey::kShift}} },
				[]()->bool { Input::SetInputMode(eInputMode::kUIOnly); return true;  });
			RegisterPressedEvent("Input Mode Editor Only", std::vector{ EventKeyInfo{eKey::k4, {eAdditionalKey::kControl, eAdditionalKey::kShift}} },
				[]()->bool { Input::SetInputMode(eInputMode::kEditor); return true;  });

			//RegisterLayer(CreateSPtr<ImGuiDemoLayer>());
			RegisterLayer(CreateSPtr<DockingLayer>());
			RegisterLayer(m_viewport_layer);
			RegisterLayer(m_menu_layer);
			RegisterLayer(m_select_level_layer);
			RegisterLayer(m_level_setting_layer);

			m_menu_layer->OnResumeLevelEvent([this]()
				{
					const auto& level = LevelManager::GetLevelManager().GetCurrentLevel();
					if (level != nullptr)
					{
						level->SetLevelState(eLevelState::kInGame);
						m_menu_layer->SetIsLevelPuase(false);
					}
				});
			m_menu_layer->OnPauseLevelEvent([this]()
				{
					const auto& level = LevelManager::GetLevelManager().GetCurrentLevel();
					if (level != nullptr)
					{
						level->SetLevelState(eLevelState::kPaused);
						m_menu_layer->SetIsLevelPuase(true);
					}
				});
			m_menu_layer->OnCloseLevelEvent([this]()
				{
					LevelManager::GetLevelManager().CloseLevel();
					m_level_setting_layer->SetCurrentSimulationLevel(nullptr);
					m_menu_layer->SetIsLevelPuase(false);
				});
			m_menu_layer->OnShutdownEvent([this]() { SetAppState(eAppState::kDead); });

			m_select_level_layer->OnOpenEvent([this](const SPtr<SimulationLevel>& level)
				{
					m_level_setting_layer->SetCurrentSimulationLevel(level);
				});
			m_select_level_layer->OnCloseEvent([this](const SPtr<SimulationLevel>& level)
				{
					m_level_setting_layer->SetCurrentSimulationLevel(nullptr);
				});

			m_select_level_layer->RegisterLevel<RotatingCubeLevel>("Rendering/Test", { u8"회전하는 큐브가 있습니다." });

			return result;
		}

		void Shutdown() override
		{
			Application::Shutdown();
		}

	private:
		SPtr<ViewportLayer> m_viewport_layer;
		SPtr<MenuLayer> m_menu_layer;
		SPtr<SelectLevelLayer> m_select_level_layer;
		SPtr<LevelSettingLayer> m_level_setting_layer;
	};
}

client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<simulation::SimulationApp>();
}