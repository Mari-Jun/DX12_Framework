#pragma once

namespace client_fw
{
	class RenderSystem;
	class RootSignature;
	class RenderLevel;
	class Shader;
	class RenderComponent;
	class SkyComponent;
	class CameraComponent;
	class RenderCameraComponent;
	class LightComponent;

	//현재 전략은 Opaque -> Deferred -> Transparent -> Compute -> UI
	//거울같은 반사는 언제 어떻게 처리를 해야 할지.. 
	enum class eRenderLevelType
	{
		kOpaque, kShadow, kShadowCube, kShadowCascade,
		kDeferred, kTransparent, kUI, kFinalView
	};

	enum class eShaderType
	{
		kOpaqueMaterialMesh, kOpaqueTextureMesh, kOpaqueNormalMapMesh, kSkeletalMesh,
		kShapeBox, 
		kTextureBillboard,
		kOpaqueMaterialBaseColorBillboard, kMaskedMaterialBaseColorBillboard,
		kOpaqueMaterialTextureBillboard, kMaskedMaterialTextureBillboard,
		kOpaqueMaterialNormalMapBillboard, kMaskedMaterialNormalMapBillboard,
		kOpaqueWidget, kMaskedWidget,
		kSky,

		kDeferred, kPointLight, kSpotLight,
	};

	enum class eKindOfRenderLevel
	{
		kGraphics, kCompute
	};

	class Render final
	{
	public:
		template <class T>
		static bool RegisterGraphicsShader(const std::string& shader_name, std::vector<eRenderLevelType>&& level_types)
		{
			return s_render_system->RegisterGraphicsShader<T>(shader_name, std::move(level_types), true);
		}

		static void UnregisterGraphicsShader(const std::string shader_name, std::vector<eRenderLevelType>&& level_types);

		static bool RegisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name);
		static void UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp, const std::string& shader_name);

		static bool RegisterSkyComponent(const SPtr<SkyComponent>& sky_comp, const std::string& shader_name);
		static void UnregisterSkyComponent(const SPtr<SkyComponent>& sky_comp, const std::string& shader_name);

		static bool RegisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		static void UnregisterCameraComponent(const SPtr<CameraComponent>& camera_comp);
		static void SetMainCamera(const SPtr<RenderCameraComponent>& camera_comp);

	public:
		static Vec2 GetWindowSize();

		static std::string ConvertRenderLevelType(eRenderLevelType type);
		static std::string ConvertShaderType(eShaderType type);

	private:
		friend RenderSystem;
		static RenderSystem* s_render_system;
	};
}


