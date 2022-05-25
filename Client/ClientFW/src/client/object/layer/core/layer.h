#pragma once
#include "client/object/core/base_object.h"

namespace client_fw
{
	enum class eLayerState
	{
		kActive, kInActive, kDead
	};

	class Layer : public IBaseObject, public std::enable_shared_from_this<Layer>
	{
	public:
		Layer(const std::string& name = "layer");
		virtual ~Layer();

		bool InitializeLayer();
		virtual bool Initialize() override { return true; }

		void ShutdownLayer();
		virtual void Shutdown() override {}

		void UpdateLayer(float delta_time);
		virtual void Update(float delta_time) override {}

	protected:
		std::string m_name;
		eLayerState m_layer_state;

	public:
		const std::string& GetName() const { return m_name; }
		eLayerState GetLayerState() const { return m_layer_state; }
		void SetLayerState(eLayerState layer_state) { m_layer_state = layer_state; }
	};


}
