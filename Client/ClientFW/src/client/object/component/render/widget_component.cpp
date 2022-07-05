#include "stdafx.h"
#include "client/object/component/render/widget_component.h"
#include "client/object/ui/core/user_interface_layer.h"
#include "client/renderer/core/render.h"

namespace client_fw
{
	WidgetComponent::WidgetComponent(const SPtr<UserInterfaceLayer>& ui_layer,
		const std::string& name, eWidgetSpaceType type, eWidgetBlendMode blend_mode)
		: RenderComponent(name, 20, eRenderType::kWidget, Render::ConvertShaderType(eShaderType::kMaskedWidget))
		, m_user_interface_layer(ui_layer), m_type(type), m_size(Vec2(200.0f, 200.0f)), m_pivot(0.5f, 0.5f)
	{
		switch (blend_mode)
		{
		case client_fw::eWidgetBlendMode::kOpaque:
			SetDrawShaderName(Render::ConvertShaderType(eShaderType::kOpaqueWidget));
			break;
		case client_fw::eWidgetBlendMode::kMasked:
			SetDrawShaderName(Render::ConvertShaderType(eShaderType::kMaskedWidget));
			break;
		case client_fw::eWidgetBlendMode::kTransparent:
			SetDrawShaderName(Render::ConvertShaderType(eShaderType::kMaskedWidget));
			break;
		}
	}

	bool WidgetComponent::Initialize()
	{
		bool ret = RenderComponent::Initialize();
		if (m_user_interface_layer != nullptr)
			ret &= m_user_interface_layer->InitializeUILayer();
		else
			ret = false;
		return ret; 
	}

	void WidgetComponent::Shutdown()
	{
		if (m_user_interface_layer != nullptr)
			m_user_interface_layer->ShutdownUILayer();
		RenderComponent::Shutdown();
	}

	void WidgetComponent::Update(float delta_time)
	{
		RenderComponent::Update(delta_time);
		if (m_user_interface_layer != nullptr)
		{
			if (m_user_interface_layer->GetUILayerState() != eUILayerState::kDead)
			{
				m_user_interface_layer->UpdateUILayer(delta_time);
			}
			else
			{
				m_user_interface_layer->Shutdown();
				m_user_interface_layer = nullptr;
			}
		}
	}

	void WidgetComponent::UpdateOrientedBox()
	{
		m_oriented_box->Transform(BOrientedBox(), GetWorldMatrix());
		float extent = m_size.Length();
		m_oriented_box->SetExtents(Vec3(extent, extent, extent));
	}

	void WidgetComponent::SetSize(const Vec2& size)
	{
		m_size = size;
		UpdateOrientedBox();
	}

	void WidgetComponent::SetPivot(const Vec2& pivot)
	{
		m_pivot.x = std::clamp(pivot.x, 0.0f, 1.0f);
		m_pivot.y = std::clamp(pivot.y, 0.0f, 1.0f);
		UpdateOrientedBox();
	}

	SPtr<WidgetComponent> WidgetComponent::SharedFromThis()
	{
		return std::static_pointer_cast<WidgetComponent>(shared_from_this());
	}
}
