#pragma once
#include "client/object/layer/core/layer.h"

namespace client_fw
{
	class ImGuiDemoLayer : public Layer
	{
	public:
		ImGuiDemoLayer(const std::string& name = "imgui demo layer");
		virtual ~ImGuiDemoLayer() = default;

		virtual void Update(float delta_time) override;
	};

}

