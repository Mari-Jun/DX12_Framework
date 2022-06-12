#pragma once
#include "client/renderer/shader/core/compute_shader.h"

namespace client_fw
{
	class RenderCameraComponent;
    class RenderCameraPostProcessingFrameResource;

    struct BlurInfo
    {
        int radius;
        std::vector<float> weights;
    };

	class RenderCameraPostProcessingShader : public ComputeShader
	{
	public:
		RenderCameraPostProcessingShader(const std::string& name);
		virtual ~RenderCameraPostProcessingShader() = default;

        virtual void Initialize(ID3D12Device* device) override;
        virtual void Shutdown() override;

        virtual void Update(ID3D12Device* device, eRenderLevelType level_type) override;
        virtual void UpdateFrameResource(ID3D12Device* device, eRenderLevelType level_type) override;
        virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const override;

        virtual D3D12_SHADER_BYTECODE CreateComputeShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;

        virtual bool CreatePipelineStates(ID3D12Device* device, const SPtr<ComputeRenderLevel>& render_level) override;

    public:
        virtual bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp) override;
        virtual void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp) override;

    private:
        std::vector<SPtr<RenderCameraComponent>> m_render_cameras;

    private:
        BlurInfo GetBlurInfo(float sigma) const;
	};
}


