#pragma once
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	class StaticMeshComponent;

	class StaticMeshActor : public Actor
	{
	public:
		StaticMeshActor(eMobilityState mobility, const std::string& static_mesh_path);
		virtual ~StaticMeshActor() = default;

		virtual bool Initialize() override;
		virtual void Shutdown() override;

	protected:
		std::string m_mesh_path;
		SPtr<StaticMeshComponent> m_static_mesh_component;
	};
}

