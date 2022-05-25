#pragma once
#include "client/object/layer/core/layer.h"

namespace simulation
{
	using namespace client_fw;

	class DockingLayer : public Layer
	{
	public:
		DockingLayer(const std::string& name = "docking layer");
		virtual ~DockingLayer() = default;

		virtual void Update(float delta_time) override;
	};

}

