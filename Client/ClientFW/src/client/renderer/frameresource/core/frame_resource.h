#pragma once

namespace client_fw
{
	class RenderResourceFrameResource;
	class CameraFrameResource;
	class SkyFrameResource;
	class StaticMeshFrameResource;	
	class SkeletalMeshFrameResource;
	class LightFrameResource;
	class ShadowFrameResource;
	class LocalLightFrameResource;
	class BillboardFrameResource;
	class WidgetFrameResource;
	class UserInterfaceFrameResource;

	enum class eRenderLevelType;

	struct ResourceOwner
	{
		std::string owner_shader_name;
		eRenderLevelType render_level_type;
	};

	bool operator<(const ResourceOwner& owner1, const ResourceOwner& owner2);

	//GPU가 사용할 데이터들을 관리하는 곳
	//CPU와 GPU의 병렬성을 위해서 필요하다.
	class FrameResource
	{
	public:
		FrameResource();
		~FrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		ComPtr<ID3D12CommandAllocator> m_command_allocator;
		UINT64 m_fence = 0;

	public:
		ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const { return m_command_allocator; }
		UINT64 GetFence() const { return m_fence; }
		void SetFence(UINT64 value) { m_fence = value; }

	private:
		UPtr<RenderResourceFrameResource> m_render_resource_frame_resource;
		UPtr<CameraFrameResource> m_camera_frame_resource;
		UPtr<LightFrameResource> m_light_frame_resource;
		UPtr<ShadowFrameResource> m_shadow_frame_resource;

	public:
		const UPtr<RenderResourceFrameResource>& GetRenderResourceFrameResource() const { return m_render_resource_frame_resource; }
		const UPtr<CameraFrameResource>& GetCameraFrameResource() const { return m_camera_frame_resource; }
		const UPtr<LightFrameResource>& GetLightFrameResource() const { return m_light_frame_resource; }
		const UPtr<ShadowFrameResource>& GetShadowFrameResource() const { return m_shadow_frame_resource; }

	private:
		std::map<ResourceOwner, UPtr<SkyFrameResource>> m_sky_frame_resource;
		std::map<ResourceOwner, UPtr<StaticMeshFrameResource>> m_static_mesh_frame_resource;
		std::map<ResourceOwner, UPtr<SkeletalMeshFrameResource>> m_skeletal_mesh_frame_resource;
		std::map<ResourceOwner, UPtr<BillboardFrameResource>> m_billboard_frame_resource;
		std::map<ResourceOwner, UPtr<WidgetFrameResource>> m_widget_frame_resource;
		std::map<ResourceOwner, UPtr<LocalLightFrameResource>> m_local_light_frame_resource;
		std::map<ResourceOwner, UPtr<UserInterfaceFrameResource>> m_ui_frame_resource;

	public:
		void CreateSkyFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type);
		void CreateStaticMeshFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type);
		void CreateSkeletalMeshFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type);
		void CreateBillboardFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type);
		void CreateWidgetFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type);
		void CreateLocalLightFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type);
		void CreateUserInterfaceFrameResource(ID3D12Device* device, const std::string& shader_name, eRenderLevelType level_type);

		const UPtr<SkyFrameResource>& GetSkyFrameResource(const std::string& shader_name, eRenderLevelType level_type) const
		{
			return m_sky_frame_resource.at({ shader_name, level_type });
		}

		const UPtr<StaticMeshFrameResource>& GetStaticMeshFrameResource(const std::string& shader_name, eRenderLevelType level_type) const
		{
			return m_static_mesh_frame_resource.at({ shader_name, level_type });
		}

		const UPtr<SkeletalMeshFrameResource>& GetSkeletalMeshFrameResource(const std::string& shader_name, eRenderLevelType level_type) const
		{
			return m_skeletal_mesh_frame_resource.at({ shader_name, level_type });
		}

		const UPtr<BillboardFrameResource>& GetBillboardFrameResource(const std::string& shader_name, eRenderLevelType level_type) const
		{
			return m_billboard_frame_resource.at({ shader_name, level_type });
		}

		const UPtr<WidgetFrameResource>& GetWidgetFrameResource(const std::string& shader_name, eRenderLevelType level_type) const
		{
			return m_widget_frame_resource.at({ shader_name, level_type });
		}

		const UPtr<LocalLightFrameResource>& GetLocalLightFrameResource(const std::string& shader_name, eRenderLevelType level_type) const
		{
			return m_local_light_frame_resource.at({ shader_name, level_type });
		}

		const UPtr<UserInterfaceFrameResource>& GetUserInterfaceFrameResource(const std::string& shader_name, eRenderLevelType level_type) const
		{
			return m_ui_frame_resource.at({ shader_name, level_type });
		}
	};
}



