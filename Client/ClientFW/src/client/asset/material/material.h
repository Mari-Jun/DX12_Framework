#pragma once
#include "client/asset/core/asset.h"

namespace client_fw
{
	class Texture;

	class Material : public Asset
	{
	public:
		Material() = default;
		virtual ~Material() = default;

	private:
		UINT m_material_resource_index = 0;
		Vec4 m_base_color;
		float m_roughness = 0.33f;
		float m_metallic = 0.0f;

	public:
		UINT GetResourceIndex() const { return m_material_resource_index; }
		void SetResourceIndex(UINT index) { m_material_resource_index = index; }
		const Vec4& GetBaseColor() const { return m_base_color; }
		void SetBaseColor(const Vec4& base_color) { m_base_color = base_color; }
		float GetRoughness() const { return m_roughness; }
		void SetRoughness(float roughness) { m_roughness = roughness; }
		float GetMetallic() const { return m_metallic; }
		void SetMetallic(float metallic) { m_metallic = metallic; }

	private:
		SPtr<Texture> m_diffuse_texture;
		SPtr<Texture> m_normal_texture;
		SPtr<Texture> m_roughness_texture;
		SPtr<Texture> m_metallic_texture;

	public:
		const SPtr<Texture>& GetDiffuseTexture() const { return m_diffuse_texture; }
		void SetDiffuseTexture(const SPtr<Texture>& texture) { m_diffuse_texture = texture; }
		const SPtr<Texture>& GetNormalTexture() const { return m_normal_texture; }
		void SetNormalTexture(const SPtr<Texture>& texture) { m_normal_texture = texture; }
		const SPtr<Texture>& GetRoughnessTexture() const { return m_roughness_texture; }
		void SetRoughnessTexture(const SPtr<Texture>& texture) { m_roughness_texture = texture; }
		const SPtr<Texture>& GetMetallicTexture() const { return m_metallic_texture; }
		void SetMetallicTexture(const SPtr<Texture>& texture) { m_metallic_texture = texture; }

	};
}


