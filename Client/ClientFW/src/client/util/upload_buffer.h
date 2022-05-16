#pragma once
#include "client/util/d3d_util.h"

namespace client_fw
{
	template<class T>
	class UploadBuffer
	{
	public:
		UploadBuffer(bool is_constant = false)
			: m_is_constant_buffer(is_constant)
		{
			if (m_is_constant_buffer)
				m_byte_size = (sizeof(T) + 255) & ~255;
			else
				m_byte_size = sizeof(T);
		}

		void Shutdown()
		{
			if (m_resource_buffer != nullptr)
			{
				m_resource_buffer->Unmap(0, nullptr);
				m_resource_buffer = nullptr;
			}
			m_mapped_data = nullptr;
			m_num_of_updated_data = 0;
		}

		void CreateResource(ID3D12Device* device, UINT num_of_data)
		{
			if (m_resource_buffer != nullptr)
				m_resource_buffer->Unmap(0, nullptr);

			m_resource_buffer = D3DUtil::CreateUploadBuffer(device, static_cast<UINT>(num_of_data * m_byte_size), &m_mapped_data);

			m_num_of_updated_data = num_of_data;
		}

		void CreateAndCopyResource(ID3D12Device* device, UINT num_of_data)
		{
			auto updated_resource_size = m_num_of_updated_data * m_byte_size;
			auto resource_size = num_of_data * m_byte_size;

			BYTE* updated_data = nullptr;
			if (updated_resource_size > 0)
			{
				updated_data = new BYTE[updated_resource_size];
				memcpy(updated_data, m_mapped_data, updated_resource_size);
			}

			if (m_resource_buffer != nullptr)
				m_resource_buffer->Unmap(0, nullptr);

			m_resource_buffer = D3DUtil::CreateUploadBuffer(device, static_cast<UINT>(resource_size), &m_mapped_data);

			if (updated_data != nullptr)
			{
				memcpy(m_mapped_data, updated_data, updated_resource_size);
				delete[] updated_data;
				updated_data = nullptr;
			}

			m_num_of_updated_data = num_of_data;
		}

		void CopyData(UINT index, const T& data)
		{
			memcpy(&m_mapped_data[index * m_byte_size], &data, sizeof(T));
		}

		void CopyVectorData(const std::vector<T>& data)
		{
			memcpy(&m_mapped_data[0], data.data(), sizeof(T) * data.size());
		}

		void CopyVectorData(std::vector<T>&& data)
		{
			memcpy(&m_mapped_data[0], data.data(), sizeof(T) * data.size());
		}

	private:
		ComPtr<ID3D12Resource> m_resource_buffer;
		BYTE* m_mapped_data = nullptr;

		UINT m_byte_size = 0;
		bool m_is_constant_buffer = false;
		UINT m_num_of_updated_data = 0;

	public:
		ID3D12Resource* GetResource() const {return m_resource_buffer.Get(); }
		BYTE* GetMappedData() { return m_mapped_data; }
		UINT GetByteSize() const { return m_byte_size; }
		UINT GetNumOfUpdatedData() const { return m_num_of_updated_data; }

	};
}


