#pragma once
#include "client/object/core/base_object.h"

namespace client_fw
{
	class Component;
	class ComponentManager;
	class ActorPhysicsManager;
	class MessageEventInfo;

	struct EventKeyInfo;
	struct AxisEventKeyInfo;

	enum class eActorState
	{
		kActive, kPaused, kDead
	};

	// Static : Runtime중에 이동하지 않고 초기화 과정에서 생성되는 Actor에 사용
	// Destructible : Runtime중에 생성/삭제는 가능하지만 이동이 불가능한 Actor에 사용
	// Movable : 그외의 Actor에 사용
	// 기본적으로 Static과 Destructible은 생성 과정에서 1번만 update함수를 호출한다.
	// update함수 호출이 매 프레임마다 필요하다면 UseUpdate() 함수를 호출해줘야 한다.
	enum class eMobilityState
	{
		kStatic, kDestructible, kMovable
	};

	class Actor : public IBaseObject, public std::enable_shared_from_this<Actor>
	{
	public:
		Actor(eMobilityState mobility = eMobilityState::kStatic, const std::string& name = "Actor");
		virtual ~Actor();

		bool InitializeActor();
		virtual bool Initialize() override { return true; }

		void ShutdownActor();
		virtual void Shutdown() override {}

		void UpdateActor(float delta_time);
		virtual void Update(float delta_time) override {}

	protected:
		virtual void RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		virtual void RegisterReleasedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		virtual void RegisterAxisEvent(const std::string& name, std::vector <AxisEventKeyInfo>&& keys,
			const std::function<bool(float)>& func, bool consumption = true);

		void RegisterInputEvent(const std::string& name);
		void RegisterReceiveMessage(UINT event_id);

	public:
		virtual void ExecuteMessage(const SPtr<MessageEventInfo>& message) {}
		virtual void ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message) {}

	public:
		void SpawnActor(const SPtr<Actor>& actor);
		bool AttachComponent(const SPtr<Component> comp);	//level에서 호출하게되면 오류가 발생한다.
		void DetachComponent(const SPtr<Component> comp);

	public:
		void UpdateWorldMatrix();

	protected:
		std::string m_name;
		eActorState m_actor_state;
		eMobilityState m_mobility_state;
		bool m_is_use_update = false;
		bool m_is_connected_to_server = false;

	private:
		std::vector<std::string> m_registered_input_events;
		std::vector<UINT> m_registered_message_events;
		UPtr<ComponentManager> m_component_manager;
		UPtr<ActorPhysicsManager> m_physics_manager;

		Mat4 m_world_matrix;
		Vec3 m_previous_position;
		Vec3 m_position;
		Quaternion m_rotation;
		Vec3 m_scale;
		bool m_update_world_matrix = true;
		bool m_is_updated_world_matrix = false;

	public:
		const std::string& GetName() const { return m_name; }
		void SetName(const std::string& name) { m_name = name; }
		eActorState GetActorState() const { return m_actor_state; }
		void SetActorState(eActorState actor_state) { m_actor_state = actor_state; }
		eMobilityState GetMobilityState() const { return m_mobility_state; }
		bool IsUseUpdate() { return m_is_use_update; }
		void UseUpdate() { m_is_use_update = true; }
		bool IsConnectedToServer() const { return m_is_connected_to_server; }
		void ConnectServer(bool value) { m_is_connected_to_server = value; }
		const UPtr<ActorPhysicsManager>& GetPhysicsManager() const { return m_physics_manager; }

		const Mat4& GetWorldMatrix() const { return m_world_matrix; }
		const Vec3& GetPreviousPosition() const { return m_previous_position; }
		const Vec3& GetPosition() const { return m_position; }
		void SetPosition(const Vec3& pos);
		const Quaternion& GetRotation() const { return m_rotation; }
		void SetRotation(const Quaternion& rot);
		void SetRotation(float pitch, float yaw, float roll);
		const Vec3& GetScale() const { return m_scale; }
		void SetScale(const Vec3& scale);
		void SetScale(float scale);
		bool IsUpdatedWorldMatrix() const { return m_is_updated_world_matrix; }

		Vec3 GetForward() const { return vec3::TransformNormal(vec3::AXIS_Z, m_rotation); }
		Vec3 GetRight() const { return vec3::TransformNormal(vec3::AXIS_X, m_rotation); }
		Vec3 GetUp() const { return vec3::TransformNormal(vec3::AXIS_Y, m_rotation); }
	};
}


