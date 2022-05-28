#include <include/client_fw.h>
//#include <client/core/application.h>
//#include <client/core/log.h>
//#include <client/input/input.h>
#include <client/core/entry_point.h>
#include "object/level/render_rect_level.h"
#include "object/level/player_test_level.h"

using namespace client_fw;

namespace render_test
{
	class RenderTestApp : public client_fw::Application
	{
	public:
		RenderTestApp() : Application(L"Render Test App")
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

			RegisterPressedEvent("open render rect level", { {eKey::k1} },
				[this]()->bool {OpenLevel(CreateSPtr<RenderRectLevel>());  return true; });
			RegisterPressedEvent("open player test level", { {eKey::k2} },
				[this]()->bool {OpenLevel(CreateSPtr<PlayerTestLevel>()); return true; });
			//Network::GetInst()->Init();
			//Network::GetInst()->CreateWorker();
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
	return client_fw::CreateUPtr<render_test::RenderTestApp>();
}