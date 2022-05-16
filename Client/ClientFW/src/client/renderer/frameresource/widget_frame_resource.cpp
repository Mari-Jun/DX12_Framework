#include "stdafx.h"
#include "client/renderer/frameresource/widget_frame_resource.h"
#include "client/asset/primitive/primitive.h"

namespace client_fw
{
	WidgetFrameResource::WidgetFrameResource()
	{
		m_world_widget_primitive = CreateUPtr<UploadPrimitive<WorldWidgetVertex>>();
		m_pivot_widget_primitive = CreateUPtr<UploadPrimitive<PivotWidgetVertex>>();
	}

	WidgetFrameResource::~WidgetFrameResource()
	{
	}

	bool WidgetFrameResource::Initialize(ID3D12Device* device)
	{
		bool ret = m_world_widget_primitive->Initialize(device);
		ret &= m_pivot_widget_primitive->Initialize(device);
		return ret;
	}

	void WidgetFrameResource::Shutdown()
	{
		m_pivot_widget_primitive->Shutdown();
		m_world_widget_primitive->Shutdown();
	}

	WorldWidgetDrawInfo WidgetFrameResource::GetWorldWidgetDrawInfo()
	{
		WorldWidgetDrawInfo info = m_world_widget_draw_info.front();
		m_world_widget_draw_info.pop();
		return info;
	}

	PivotWidgetDrawInfo WidgetFrameResource::GetPivotWidgetDrawInfo()
	{
		PivotWidgetDrawInfo info = m_pivot_widget_draw_info.front();
		m_pivot_widget_draw_info.pop();
		return info;
	}
}
