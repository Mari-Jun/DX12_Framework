#pragma once
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	class SkyCubeComponent;

	class SkyCube : public Actor
	{
	public:
		SkyCube(const std::string& texture_path = "", const std::string& name = "sky cube");
		virtual ~SkyCube() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	protected:
		std::string m_texture_path;
		SPtr<SkyCubeComponent> m_sky_cube_component;

	public:
		const std::string& GetTexturePath() const { return m_texture_path; }
		void SetTexturePath(const std::string& path) { m_texture_path = path; }
	};

}

