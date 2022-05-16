#pragma once

namespace client_fw
{
	template<class VertexType>
	class UploadPrimitive;
	class BillboardVertex;

	// 한 카메라가 그리는 billboard들의 정보가 담긴 구조체
	// 카메라가 여러개이면 start_index는 중첩된다.
	// 예를 들어 카메라가 2개이고 각 그리는 billboard, fixupbillboard의 수가
	// (3개, 5개) (10개, 2개)라면
	// 정보는 {0, 3, 3, 5} {8, 10, 18, 2} 이렇게 저장된다.
	struct BillboardDrawInfo
	{
		UINT start_index;
		UINT num_of_draw_data;
		UINT fix_up_start_index;
		UINT num_of_draw_fix_up_data;
	};

	class BillboardFrameResource
	{
	public:
		BillboardFrameResource();
		~BillboardFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadPrimitive<BillboardVertex>> m_billboard_primitive;

		UINT m_size_of_billboard_primitive = 1;

		std::queue<BillboardDrawInfo> m_draw_info;

	public:
		const UPtr<UploadPrimitive<BillboardVertex>>& GetBillboardPrimitive() const { return m_billboard_primitive; }

		UINT GetSizeOfBillboardPrimitive() const { return m_size_of_billboard_primitive; }
		void SetSizeOfBillboardPrimitive(UINT value) { m_size_of_billboard_primitive = value; }

		void AddBillboardDrawInfo(BillboardDrawInfo&& info) { m_draw_info.emplace(std::move(info)); }

		BillboardDrawInfo GetBillboardDrawInfo();
	};
}



