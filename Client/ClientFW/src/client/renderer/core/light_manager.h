#pragma once

namespace client_fw
{
	class LightComponent;
	class DirectionalLightComponent;
	class PointLightComponent;
	class SpotLightComponent;

	constexpr static UINT s_max_directional_light = 4;

	// Light의 정보를 GPU에서 사용하기 위해 Light를 관리하는 클래스
	class LightManager final
	{
	public:
		LightManager();
		~LightManager();

		LightManager(const LightManager&) = delete;
		LightManager(LightManager&&) = delete;
		LightManager& operator=(const LightManager&) = delete;
		LightManager& operator=(LightManager&&) = delete;

		void Shutdown();
		void Update(ID3D12Device* device);

		void Draw(ID3D12GraphicsCommandList* command_list);

		bool RegisterLightComponent(const SPtr<LightComponent>& light_comp);
		void UnregisterLightComponent(const SPtr<LightComponent>& light_comp);

	private:
		template <class T>
		void RegisterLightComponent(std::vector<SPtr<T>>& lights, const SPtr<LightComponent>& light_comp)
		{
			lights.push_back(std::static_pointer_cast<T>(light_comp));
			++m_num_of_light;
			if(light_comp->IsUseShadow())
				++m_num_of_shadow_texture;
		}

		template <class T>
		void UnregisterLightComponent(std::vector<SPtr<T>>& lights, const SPtr<LightComponent>& light_comp)
		{
			auto iter = std::find(lights.begin(), lights.end(), light_comp);
			if (iter != lights.end())
			{
				std::iter_swap(iter, lights.end() - 1);
				lights.pop_back();
				--m_num_of_light;
				if(light_comp->IsUseShadow())
					--m_num_of_shadow_texture;
			}
		}
			
	private:
		static LightManager* s_light_manager;
		UINT m_num_of_light = 0;
		UINT m_num_of_shadow_texture = 0;
		UINT m_num_of_cascade_shadow_texture = 0;

		std::vector<SPtr<DirectionalLightComponent>> m_ready_directional_lights;
		std::vector<SPtr<DirectionalLightComponent>> m_directional_lights;
		std::vector<SPtr<PointLightComponent>> m_point_lights;
		std::vector<SPtr<SpotLightComponent>> m_spot_lights;

	public:
		static LightManager& GetLightManager() { return *s_light_manager; }
		UINT GetNumOfLight() const { return m_num_of_light; }

		const std::vector<SPtr<DirectionalLightComponent>>& GetReadyDirectionalLights() const { return m_ready_directional_lights; }
		const std::vector<SPtr<DirectionalLightComponent>>& GetDirectionalLights() const { return m_directional_lights; }

	private:
		static std::array<Mat4, 1> s_uv_from_ndc_matrix;
	};
}



