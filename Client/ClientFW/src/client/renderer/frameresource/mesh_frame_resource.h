#pragma once

namespace client_fw
{
	template<class T> class UploadBuffer;
	class Mesh;

	struct RSInstanceData
	{
		Mat4 world_matrix;
		Mat4 world_inverse_transpose;
		UINT m_additional_info = 0;
	};

	struct RSSkeletalData
	{
		Mat4 bone_transform;
	};

	//
	// 각 카메라마다 그리는 Mesh에 대해서 적용되는 DrawInfo
	// mesh는 mesh정보
	// draw start index는 각 카메라가 mesh에 대해서 그릴 때 instance data의 시작 위치를 표기한 것
	// 예를 들어 1번 카메라가 mesh 3종류를 각 3개씩 그린다고 가정을 하면
	// draw start index는 0, 3, 6 순으로 저장된다. 
	// (2번 카메라여도 동일하게 3종류씩 각 3개씩 그린다면 같은 값이 저장된다)
	// start index of lod instance data는 각 mesh마다 lod가 존재하는데 그 lod의 시작 위치를 저장한다.
	// num of lod instance data는 lod의 수이다.
	// 위의 예로 설명을 하자면 mesh중 첫번째 종류의 mesh가 lod가 0이 1개 1이 1개 2가 1개가 존재한다고 가정을 하게되면
	// start index of lod instance data : {0, 1, 2}
	// num of lod instatnce data : {1, 1, 1} 
	// 이렇게 저장이 된다.
	// 
	struct MeshDrawInfo
	{
		SPtr<Mesh> mesh;
		INT draw_start_index;
		std::vector<UINT> start_index_of_lod_instance_data;
		std::vector<UINT> num_of_lod_instance_data;
	};

	//
	//카메라마다 적용되는 DrawInfo
	//shader마다 render item과 frameresource를 가지는데
	//카메라가 여러개면 update함수와 draw함수가 여러번 호출됨에 반에
	//frameresource는 하나이기 때문에 필요한 정보들
	// start index는 각 카메라가 그리는 mesh들의 instance data의 가장 첫번 째 인덱스
	// num of instancedata는 각 카메라가 그리는 mesh들의 수를 저장함
	// 예를들어 카메라가 2개 있고 첫번째 카메라가 10개, 두번째 카메라가 5개의 mesh를 그린다고하면
	// 정보는 각각 {0,10}, {10,5} 이렇게 저장됨
	//
	struct MeshesInstanceDrawInfo
	{
		UINT start_index;
		UINT num_of_instnace_data;
		std::vector<MeshDrawInfo> mesh_draw_infos;
	};

	class MeshFrameResource
	{
	public:
		MeshFrameResource();
		virtual ~MeshFrameResource();

		virtual bool Initialize(ID3D12Device* device) { return true; }
		virtual void Shutdown() {}

	private:
		UINT m_size_of_instance_data = 1;

		std::queue<MeshesInstanceDrawInfo> m_meshes_instance_draw_info;

	public:
		UINT GetSizeOfInstanceData() const { return m_size_of_instance_data; }
		void SetSizeOfInstanceData(UINT value) { m_size_of_instance_data = value; }

		void AddMeshesInstanceDrawInfo(MeshesInstanceDrawInfo&& info) { m_meshes_instance_draw_info.emplace(std::move(info)); }

		MeshesInstanceDrawInfo GetMeshesInstanceDrawInfo();
	};

	class StaticMeshFrameResource : public MeshFrameResource
	{
	public:
		StaticMeshFrameResource();
		virtual ~StaticMeshFrameResource();

		virtual bool Initialize(ID3D12Device* device);
		virtual void Shutdown();
			
	private:
		UPtr<UploadBuffer<RSInstanceData>> m_instance_data;

	public:
		const UPtr<UploadBuffer<RSInstanceData>>& GetInstanceData() const { return m_instance_data; }
	};

	class SkeletalMeshFrameResource : public MeshFrameResource
	{
	public:
		SkeletalMeshFrameResource();
		virtual ~SkeletalMeshFrameResource();

		virtual bool Initialize(ID3D12Device* device);
		virtual void Shutdown();

	private:
		UINT m_size_of_skeletal_transform_data = 1;

		UPtr<UploadBuffer<RSInstanceData>> m_skeletal_instance_data;
		UPtr<UploadBuffer<RSSkeletalData>> m_skeletal_transform_data;

	public:
		UINT GetSizeOfSkeletalTransformData() const { return m_size_of_skeletal_transform_data; }
		void SetSizeOfSkeletalTransformData(UINT value) { m_size_of_skeletal_transform_data = value; }

		const UPtr<UploadBuffer<RSInstanceData>>& GetInstanceData() const { return m_skeletal_instance_data; }
		const UPtr<UploadBuffer<RSSkeletalData>>& GetSkeletalTransformData() const { return m_skeletal_transform_data; }
	};
}


