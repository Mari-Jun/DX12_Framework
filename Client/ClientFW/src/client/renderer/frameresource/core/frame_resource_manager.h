#pragma once

namespace client_fw
{
	constexpr UINT s_num_of_frame_resource = 3;

	class FrameResource;

	//GPU가 사용할 데이터들을 Frmae(3개)마다 관리하는 곳
	//CPU와 GPU의 병렬성을 위해서 필요하다.
	class FrameResourceManager
	{
	public:
		FrameResourceManager();
		~FrameResourceManager();

		FrameResourceManager(const FrameResourceManager&) = delete;
		FrameResourceManager& operator=(const FrameResourceManager&) = delete;

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		static FrameResourceManager* s_resource_manager;
		std::array<UPtr<FrameResource>, s_num_of_frame_resource> m_frame_resources;

		UINT m_current_frame = 0;

	public:
		static FrameResourceManager& GetManager() { return *s_resource_manager; }
		const UPtr<FrameResource>& GetCurrentFrameResource() const { return m_frame_resources[m_current_frame]; }
		const std::array<UPtr<FrameResource>, s_num_of_frame_resource>& GetFrameResources() const { return m_frame_resources; }
		void MoveToNextFrame() { m_current_frame = (m_current_frame + 1) % s_num_of_frame_resource; }
	};
}


