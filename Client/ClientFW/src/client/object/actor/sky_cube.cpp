#include "stdafx.h"
#include "client/object/actor/sky_cube.h"
#include "client/object/component/sky/sky_cube_component.h"

namespace client_fw
{
	SkyCube::SkyCube(const std::string& texture_path, const std::string& name)
		: Actor(eMobilityState::kStatic, name), m_texture_path(texture_path)
	{
		m_sky_cube_component = CreateSPtr<SkyCubeComponent>();
	}

	bool SkyCube::Initialize()
	{
		bool ret = true;
		ret &= AttachComponent(m_sky_cube_component);
		m_sky_cube_component->SetCubeMapTexture(m_texture_path);
		return ret;
	}

	void SkyCube::Shutdown()
	{
		m_sky_cube_component = nullptr;
	}
}
