#include <include/client_fw.h>
#include <client/core/entry_point.h>

#include "object/level/rotating_cube_level.h"

#include "object/layer/imgui_demo_layer.h"
#include "object/layer/viewport_layer.h"

using namespace client_fw;

namespace simulation
{
	class SimulationApp : public client_fw::Application
	{
	public:
		SimulationApp() : Application(L"Simulation App")
		{
		}

		bool Initialize() override
		{
			bool result = Application::Initialize();

			RegisterPressedEvent("Input Mode Game Only", std::vector{ EventKeyInfo{eKey::k1, {eAdditionalKey::kControl}} },
				[]()->bool { Input::SetInputMode(eInputMode::kGameOnly); return true;  });
			RegisterPressedEvent("Input Mode Game And UI", std::vector{ EventKeyInfo{eKey::k2, {eAdditionalKey::kControl}} },
				[]()->bool { Input::SetInputMode(eInputMode::kUIAndGame); return true;  });
			RegisterPressedEvent("Input Mode UI Only", std::vector{ EventKeyInfo{eKey::k3, {eAdditionalKey::kControl}} },
				[]()->bool { Input::SetInputMode(eInputMode::kUIOnly); return true;  });

			RegisterPressedEvent("open rotating cube level", { {eKey::k1} },
				[this]()->bool {OpenLevel(CreateSPtr<RotatingCubeLevel>()); return true; });

			//RegisterLayer(CreateSPtr<ImGuiDemoLayer>());
			RegisterLayer(CreateSPtr<ViewportLayer>());

			return result;
		}

		void Shutdown() override
		{
			Application::Shutdown();
		}
	};
}

client_fw::UPtr<client_fw::Application> client_fw::CreateApplication()
{
	return client_fw::CreateUPtr<simulation::SimulationApp>();
}