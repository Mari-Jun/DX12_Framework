#pragma once

namespace client_fw
{
	static constexpr float s_gravity_acceleration = 980.0f;
	static constexpr float s_max_gravity_speed = 6000.0f;

	class Actor;

	class ActorPhysicsManager final
	{
	public:
		ActorPhysicsManager();
		~ActorPhysicsManager() = default;

		ActorPhysicsManager(const ActorPhysicsManager&) = delete;
		ActorPhysicsManager& operator=(const ActorPhysicsManager&) = delete;

		void Update(float delta_time);

	private:
		void UpdateGravity(float delta_time);

	private:
		UINT m_is_active = 0;
		WPtr<Actor> m_owner;
		float m_gravity_speed = 0.0f;

		Vec3 m_force;
		float m_mass = 1.0f;
		Vec3 m_acceleration;

	public:
		bool IsActive() const { return m_is_active > 0; }
		void SetActive(bool active) { active ? ++m_is_active : --m_is_active; }
		void SetOwner(const WPtr<Actor>& owner) { m_owner = owner; }
		void ResetGravitySpeed() { m_gravity_speed = 0.0f; }
	};

}


