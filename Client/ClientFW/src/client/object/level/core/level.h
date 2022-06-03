#pragma once
#include "client/object/core/base_object.h"

namespace client_fw
{
	class Actor;
	class ActorManager;
	class GameMode;
	class UserInterfaceLayer;
	struct EventKeyInfo;
	struct AxisEventKeyInfo;
	class VisualOctree;
	class CollisionOctree;
	class MessageEventInfo;

	enum class eLevelState
	{
		kInGame, kPaused, kDead
	};

	class Level : public IBaseObject, public std::enable_shared_from_this<Level>
	{
	public:
		Level(const std::string& name = "level");
		virtual ~Level();

		bool InitializeLevel();
		virtual bool Initialize() override { return true; }

		void ShutdownLevel();
		virtual void Shutdown() override {}

		void UpdateLevel(float delta_time);
		virtual void Update(float delta_time) override {}
		void UpdateWorldMatrix();

		void SpawnActor(const SPtr<Actor>& actor) const;
		void RegisterUILayer(const SPtr<UserInterfaceLayer>& ui_layer) const;

	protected:
		void RegisterPressedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		void RegisterReleasedEvent(const std::string& name, std::vector<EventKeyInfo>&& keys,
			const std::function<bool()>& func, bool consumption = true);
		void RegisterAxisEvent(const std::string& name, std::vector <AxisEventKeyInfo>&& keys,
			const std::function<bool(float)>& func, bool consumption = true);

	private:
		void RegisterInputEvent(const std::string& name);

	public:
		virtual void ExecuteMessage(const SPtr<MessageEventInfo>& message) {}
		virtual void ExecuteMessageFromServer(const SPtr<MessageEventInfo>& message) {}

	protected:
		std::string m_name;
		eLevelState m_level_state;

		// �⺻ ���� GameMode�� �ƴ� ��� ���� ����Ϸ��� �� Level�� �����ڿ��� ���Ӱ� ������ּ���.
		UPtr<GameMode> m_game_mode;

	private:
		std::vector<std::string> m_registered_input_events;
		UPtr<ActorManager> m_actor_manager;
		bool m_is_runtime_level;

	public:
		const std::string& GetName() const { return m_name; }
		eLevelState GetLevelState() const { return m_level_state; }
		void SetLevelState(eLevelState level_state) { m_level_state = level_state; }
		void SetRuntime() { m_is_runtime_level = true; }
		bool IsRuntime() const { return m_is_runtime_level; }
		const UPtr<GameMode>& GetGameMode() const { return m_game_mode; }

		// Level�� rendering �Ǵ� ������ collision�Ǵ� ������ �����Ѵ�.
		// �ý��� ���������� Octree�� ���ؼ� ������ ����, �浹 ���θ� �Ǵ��ϴµ�, 
		// Octree�� �⺻������ ������ü�̱� ������ ������ü�� �����ϱ� ���ؼ� vector�� ����
		// �������� Octree�� �����ϵ��� �ϰ� �ִ�.
		virtual std::vector<SPtr<VisualOctree>> CreateVisualOctrees() const;
		virtual std::vector<SPtr<CollisionOctree>> CreateCollisionOctrees() const;
	};
}

