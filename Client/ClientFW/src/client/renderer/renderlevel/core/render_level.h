#pragma once

namespace client_fw
{
	class GraphicsSuperRootSignature;
	class Shader;
	class GraphicsShader;
	class ComputeSuperRootSignature;
	class ComputeShader;
	class MeshComponent;
	class CameraComponent;
	class RenderCameraComponent;
	enum class eRenderLevelType;

	class RenderLevel
	{
	protected:
		RenderLevel(eRenderLevelType level_type);
		virtual ~RenderLevel() = default;

	public:
		virtual bool Initialize(ID3D12Device* device) { return true; }
		virtual void Shutdown() {}
		virtual void Update(ID3D12Device* device) = 0;
		virtual void UpdateFrameResource(ID3D12Device* device) = 0;

	protected:
		eRenderLevelType m_level_type;

	public:
		eRenderLevelType GetRenderLevelType() const { return m_level_type; }
	};

	class GraphicsRenderLevel : public RenderLevel, public std::enable_shared_from_this<GraphicsRenderLevel>
	{
	protected:
		GraphicsRenderLevel(eRenderLevelType level_type, const SPtr<GraphicsSuperRootSignature>& root_signature,
			std::vector<DXGI_FORMAT>&& rtv_formats, DXGI_FORMAT dsv_format);
		virtual ~GraphicsRenderLevel() = default;

	public:
		virtual void Shutdown() override;
		virtual void Update(ID3D12Device* device) override;
		virtual void UpdateFrameResource(ID3D12Device* device) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list) const;

		bool RegisterGraphicsShader(ID3D12Device* device, const SPtr<GraphicsShader>& graphics_shader);
		void UnregisterGraphicsShader(const SPtr<GraphicsShader>& graphics_shader);

	private:
		SPtr<GraphicsSuperRootSignature> m_graphics_root_signature;
		std::vector<SPtr<GraphicsShader>> m_graphics_shaders;
		std::vector<DXGI_FORMAT> m_rtv_formats;
		DXGI_FORMAT m_dsv_format;

	public:
		const std::vector<DXGI_FORMAT>& GetRTVFormats() const { return m_rtv_formats; }
		DXGI_FORMAT GetDSVFormat() const { return m_dsv_format; }
		const SPtr<GraphicsSuperRootSignature>& GetRootSignature() const { return m_graphics_root_signature; }
	};

	class ComputeRenderLevel : public RenderLevel, public std::enable_shared_from_this<ComputeRenderLevel>
	{
	protected:
		ComputeRenderLevel(eRenderLevelType level_type, const SPtr<ComputeSuperRootSignature>& root_signature);
		virtual ~ComputeRenderLevel() = default;

	public:
		virtual void Shutdown() override;
		virtual void Update(ID3D12Device* device) override;
		virtual void UpdateFrameResource(ID3D12Device* device) override;
		virtual void Draw(ID3D12GraphicsCommandList* command_list) const;

		bool RegisterComputeShader(ID3D12Device* device, const SPtr<ComputeShader>& compute_shader);
		void UnregisterComputeShader(const SPtr<ComputeShader>& compute_shader);

		bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);

	private:
		SPtr<ComputeSuperRootSignature> m_compute_root_signature;
		std::vector<SPtr<ComputeShader>> m_compute_shaders;

	public:
		const SPtr<ComputeSuperRootSignature>& GetRootSignature() const { return m_compute_root_signature; }
	};

}