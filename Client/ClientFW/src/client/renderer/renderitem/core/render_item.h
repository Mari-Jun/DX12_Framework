#pragma once

namespace client_fw
{
	enum class eRenderLevelType;

	class RenderItem
	{
	protected:
		RenderItem(const std::string& owner_shader_name);
		virtual ~RenderItem() {}

	public:
		virtual void Initialize(ID3D12Device* device, const std::vector<eRenderLevelType>& level_types) = 0;
		virtual void Shutdown() {}

		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) = 0;
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) = 0;

	protected:
		std::string m_owner_shader_name;
	};
}



