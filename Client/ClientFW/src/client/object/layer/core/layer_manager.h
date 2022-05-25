#pragma once
#include "client/object/core/base_object_manager.h"

namespace client_fw
{
	class Layer;

	class LayerManager final : public IBaseObjectManager
	{
	public:
		LayerManager();
		virtual ~LayerManager();

		LayerManager(const LayerManager&) = delete;
		LayerManager& operator=(const LayerManager&) = delete;
		LayerManager(LayerManager&&) = delete;
		LayerManager& operator=(LayerManager&&) = delete;

		virtual void Shutdown() override;
		virtual void Update(float delta_time) override;

		void RegisterLayer(const SPtr<Layer>& layer);

	private:
		static LayerManager* s_instance;

		std::vector<SPtr<Layer>> m_ready_layers;
		std::vector<SPtr<Layer>> m_layers;

		std::map<std::string, SPtr<Layer>> m_layers_map;

	public:
		inline static LayerManager& GetLayerManager() { return *s_instance; }
		SPtr<Layer> GetLayer(const std::string& layer_name);
	};
}

