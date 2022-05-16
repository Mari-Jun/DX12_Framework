#pragma once

namespace client_fw
{
	template<class VertexType>
	class UploadPrimitive;
	class UIVertex;

	class UserInterfaceFrameResource
	{
	public:
		UserInterfaceFrameResource();
		~UserInterfaceFrameResource();

		bool Initialize(ID3D12Device* device);
		void Shutdown();

	private:
		UPtr<UploadPrimitive<UIVertex>> m_ui_primitive;

		UINT m_size_of_ui_primitive = 1;

	public:
		const UPtr<UploadPrimitive<UIVertex>>& GetUserInterfacePrimitive() const { return m_ui_primitive; }

		UINT GetSizeOfUIPrimitive() const { return m_size_of_ui_primitive; }
		void SetSizeOfUIPrimitive(UINT value) { m_size_of_ui_primitive = value; }
	};
}