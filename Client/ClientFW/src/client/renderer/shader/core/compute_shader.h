#pragma once
#include "client/renderer/shader/core/shader.h"

namespace client_fw
{
	class ComputeRenderLevel;
	class CameraComponent;

	class ComputeShader : public Shader
	{
	protected:
		ComputeShader(const std::string& name);
		virtual ~ComputeShader() = default;

	public:
		virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override {}
		virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override {}
		virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const override {}

	public:
		virtual D3D12_SHADER_BYTECODE CreateComputeShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const;
		virtual bool CreatePipelineStates(ID3D12Device* device, const SPtr<ComputeRenderLevel>& render_level) = 0;

	protected:
		bool CreatePipelineState(ID3D12Device* device, const SPtr<ComputeRenderLevel>& render_level, int num_of_pso);

	public:
		virtual bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		virtual void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
	};
}


