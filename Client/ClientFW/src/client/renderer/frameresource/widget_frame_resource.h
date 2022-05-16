#pragma once

namespace client_fw
{
	template<class VertexType>
	class UploadPrimitive;
	class WorldWidgetVertex;
	class PivotWidgetVertex;

	// �� ī�޶� �׸��� world widget���� ������ ��� ����ü
	// ī�޶� �������̸� start_index�� ��ø�ȴ�.
	// ���� ��� ī�޶� 2���̰� �� �׸��� widget�� ����
	// 3��, 2�����
	// ������ {0, 3}, {3, 2} �̷��� ����ȴ�.
	struct WorldWidgetDrawInfo
	{
		UINT start_index;
		UINT num_of_draw_data;
	};

	// �� ī�޶� �׸��� billboard widget���� ������ ��� ����ü
	// ī�޶� �������̸� start_index�� ��ø�ȴ�.
	// ���� ��� ī�޶� 2���̰� �� �׸��� billboard, fixupbillboard�� ����
	// (3��, 5��) (10��, 2��)���
	// ������ {0, 3, 3, 5} {8, 10, 18, 2} �̷��� ����ȴ�.
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

