#pragma once
#include "client/renderer/shader/core/compute_shader.h"

namespace client_fw
{
    class RenderCameraComponent;

    class TestPostProcessShader final : public ComputeShader
    {
    public:
        TestPostProcessShader(const std::string& name);
        virtual ~TestPostProcessShader() = default;

        virtual void Initialize(ID3D12Device* device) override;
        virtual void Shutdown() override;

        virtual void Draw(ID3D12GraphicsCommandList* command_list, eRenderLevelType level_type) const override;

        virtual D3D12_SHADER_BYTECODE CreateComputeShader(ID3DBlob** shader_blob, eRenderLevelType level_type, int pso_index) const override;

        virtual bool CreatePipelineStates(ID3D12Device* device, const SPtr<ComputeRenderLevel>& render_level) override;

    public:
        virtual bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp) override;
        virtual void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp) override;

    private:
        std::vector<SPtr<RenderCameraComponent>> m_render_cameras;
    };
}



