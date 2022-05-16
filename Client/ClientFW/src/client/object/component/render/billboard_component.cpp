#include "stdafx.h"
#include "client/object/component/render/billboard_component.h"

namespace client_fw
{
	BillboardComponent::BillboardComponent(const std::string& name,
		eBillboardType type, const std::string& draw_shader_name)
		: RenderComponent(name, 20, eRenderType::kBillboard, draw_shader_name)
		, m_type(type), m_size(Vec2(100.f, 100.f))
	{
	}

	void BillboardComponent::UpdateOrientedBox()
	{
		m_oriented_box->Transform(BOrientedBox(), GetWorldMatrix());
		float extent = m_size.Length() * 0.5f;
		m_oriented_box->SetExtents(Vec3(extent, extent, extent));
	}

	void BillboardComponent::SetSize(const Vec2& size)
	{
		m_size = size;
		UpdateOrientedBox();
	}

	SPtr<BillboardComponent> BillboardComponent::SharedFromThis()
	{
		return std::static_pointer_cast<BillboardComponent>(shared_from_this());
	}
}
