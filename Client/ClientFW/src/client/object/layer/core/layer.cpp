#include "stdafx.h"
#include "client/object/layer/core/layer.h"

namespace client_fw
{
	Layer::Layer(const std::string& name)
		: m_name(name), m_layer_state(eLayerState::kActive)
	{
	}

	Layer::~Layer()
	{
	}

	bool Layer::InitializeLayer()
	{
		bool ret = true;
		ret &= Initialize();
		return ret;
	}

	void Layer::ShutdownLayer()
	{
		Shutdown();
	}

	void Layer::UpdateLayer(float delta_time)
	{
		Update(delta_time);
	}
}
