#include <include/client_core.h>
#include <include/dx12_shader_fw.h>
#include <client/input/input.h>
#include <client/renderer/core/render.h>
#include <client/renderer/core/render_system.h>
#include <client/renderer/renderlevel/core/render_level.h>
#include "render/shader/render_rect_shader.h"
#include "object/level/render_rect_level.h"
#include "object/actor/rect_actor.h"

#include <random>

namespace render_test
{
	RenderRectLevel::RenderRectLevel(const std::string& name)
		: Level(name)
	{
	}

	bool RenderRectLevel::Initialize()
	{
		RegisterPressedEvent("spawn rect actor", { EventKeyInfo{eKey::kS} },
			[this]()->bool {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_real_distribution<float> uid_position(-0.5f, 0.5f);
				auto actor = CreateSPtr<RectActor>();
				SpawnActor(actor);
				actor->SetPosition(Vec3(uid_position(gen), uid_position(gen), 0.01f));
				m_actors.push(actor);
				return true;
			});

		RegisterPressedEvent("kill rect actor", { EventKeyInfo{eKey::kD} },
			[this]()->bool {
				if (m_actors.empty() == false)
				{
					m_actors.front()->SetActorState(eActorState::kDead);
					m_actors.pop();
				}
				return true;
			});


		Render::RegisterGraphicsShader<RenderRectShader>("render rect shader", { eRenderLevelType::kOpaque });

		return true;
	}

	void RenderRectLevel::Shutdown()
	{
		Render::UnregisterGraphicsShader("render rect shader", { eRenderLevelType::kOpaque });
	}

	void RenderRectLevel::Update(float delta_time)
	{
	}
}
