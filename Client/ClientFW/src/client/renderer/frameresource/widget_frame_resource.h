#pragma once

namespace client_fw
{
	template<class VertexType>
	class UploadPrimitive;
	class WorldWidgetVertex;
	class PivotWidgetVertex;

	// 한 카메라가 그리는 world widget들의 정보가 담긴 구조체
	// 카메라가 여러개이면 start_index는 중첩된다.
	// 예를 들어 카메라가 2개이고 각 그리는 widget의 수가
	// 3개, 2개라면
	// 정보는 {0, 3}, {3, 2} 이렇게 저장된다.
	struct WorldWidgetDrawInfo
	{
		UINT start_index;
		UINT num_of_draw_data;
	};

	// 한 카메라가 그리는 billboard widget들의 정보가 담긴 구조체
	// 카메라가 여러개이면 start_index는 중첩된다.
	// 예를 들어 카메라가 2개이고 각 그리는 billboard, fixupbillboard의 수가
	// (3개, 5개) (10개, 2개)라면
	// 정보는 {0, 3, 3, 5} {8, 10, 18, 2} 이렇게 저장된다.
	struct PivotWidgetDrawInfo
	{
		UINT billboard_start_index;
		UINT num_of_draw_billboard_data;
		UINT fix_up_start_index;
		UINT num_of_draw_fix_up_data;
	};

	class WidgetFrameResource
	{
	public:
		WidgetFrameResource();
		~WidgetFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadPrimitive<WorldWidgetVertex>> m_world_widget_primitive;
		UPtr<UploadPrimitive<PivotWidgetVertex>> m_pivot_widget_primitive;

		UINT m_size_of_world_widget_primitive = 1;
		UINT m_size_of_pivot_widget_primitive = 1;

		std::queue<WorldWidgetDrawInfo> m_world_widget_draw_info;
		std::queue<PivotWidgetDrawInfo> m_pivot_widget_draw_info;

	public:
		const UPtr<UploadPrimitive<WorldWidgetVertex>>& GetWorldWidgetPrimitive() const { return m_world_widget_primitive; }
		const UPtr<UploadPrimitive<PivotWidgetVertex>>& GetPivotWidgetPrimitive() const { return m_pivot_widget_primitive; }

		UINT GetSizeOfWorldWidgetPrimitive() const { return m_size_of_world_widget_primitive; }
		UINT GetSizeOfPivotWidgetPrimitive() const { return m_size_of_pivot_widget_primitive; }
		void SetSizeOfWorldWidgetPrimitive(UINT value) { m_size_of_world_widget_primitive = value; }
		void SetSizeOfPivotWidgetPrimitive(UINT value) { m_size_of_pivot_widget_primitive = value; }

		void AddWorldWidgetDrawInfo(WorldWidgetDrawInfo&& info) { m_world_widget_draw_info.emplace(std::move(info)); }
		void AddPivotWidgetDrawInfo(PivotWidgetDrawInfo&& info) { m_pivot_widget_draw_info.emplace(std::move(info)); }

		WorldWidgetDrawInfo GetWorldWidgetDrawInfo();
		PivotWidgetDrawInfo GetPivotWidgetDrawInfo();
	};

}

