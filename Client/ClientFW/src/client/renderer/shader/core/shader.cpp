#include "stdafx.h"
#include "client/renderer/shader/core/shader.h"
#include "client/renderer/core/render.h"
#include "client/renderer/renderlevel/core/render_level.h"

namespace client_fw
{
	Shader::Shader(const std::string& name)
		: m_name(name)
	{
	}

	D3D12_SHADER_BYTECODE Shader::CompileShader(std::wstring_view file_name, std::string_view entry_point,
		std::string_view version, ID3DBlob** shader_blob) const
	{
		UINT compile_flags = 0; 
#ifdef _DEBUG
		compile_flags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		compile_flags |= D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;

		//std::wstring file_path = file_name.data();
		
		ID3DBlob* error;

		D3DCompileFromFile(file_name.data(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry_point.data(), version.data(),
			compile_flags, 0, shader_blob, &error);

		if (error != nullptr)
		{
			OutputDebugStringA(static_cast<char*>(error->GetBufferPointer()));
			error->Release();
		}

		return D3D12_SHADER_BYTECODE { (*shader_blob)->GetBufferPointer(), (*shader_blob)->GetBufferSize() };
	}

	void Shader::DebugPipelineState(eRenderLevelType level_type, int index) const
	{
		LOG_ERROR("Could not create {0}'s {1} PSO [{2}]", m_name, Render::ConvertRenderLevelType(level_type), index);
	}

	void Shader::AddRegisteredRenderLevelType(eRenderLevelType level_type)
	{
		m_registered_render_levels.push_back(level_type);
	}
}