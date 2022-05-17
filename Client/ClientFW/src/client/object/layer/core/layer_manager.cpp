#include "stdafx.h"
#include "client/object/layer/core/layer_manager.h"
#include "client/object/layer/core/layer.h"

namespace client_fw
{
	LayerManager::LayerManager()
	{
	}

	LayerManager::~LayerManager()
	{
	}

	void LayerManager::Shutdown()
	{
		for (const auto& layer : m_ready_layers)
			layer->ShutdownLayer();
		for (const auto& layer : m_layers)
			layer->ShutdownLayer();

		m_ready_layers.clear();
		m_layers.clear();
		m_layers_map.clear();
	}

	void LayerManager::Update(float delta_time)
	{
		for (auto& layer : m_ready_layers)
		{
			if (layer->InitializeLayer())
				m_layers.push_back(layer);
		}

		m_ready_layers.clear();

		int dead_count = 0;

		for (auto i_layer = m_layers.begin(); i_layer != m_layers.end() - dead_count;)
		{
			const auto& layer = (*i_layer);
			if (layer->GetLayerState() == eLayerState::kDead)
			{
				layer->ShutdownLayer();
				m_layers_map.erase(layer->GetName());
				std::iter_swap(i_layer, m_layers.rbegin() + dead_count);
				++dead_count;
			}
			else
			{
				layer->UpdateLayer(delta_time);
				++i_layer;
			}
		}

		while (dead_count--)
			m_layers.pop_back();
	}

	void LayerManager::RegisterLayer(const SPtr<Layer>& layer)
	{
		if (m_layers_map.find(layer->GetName()) == m_layers_map.cend())
		{
			m_ready_layers.push_back(layer);
			m_layers_map[layer->GetName()] = layer;
		}
		else
		{
			LOG_WARN("The layer name[{0}] is duplicated", layer->GetName());
		}
	}

	SPtr<Layer> LayerManager::GetLayer(const std::string& layer_name)
	{
		if (m_layers_map.find(layer_name) != m_layers_map.cend())
			return m_layers_map[layer_name];
		return nullptr;
	}
}
