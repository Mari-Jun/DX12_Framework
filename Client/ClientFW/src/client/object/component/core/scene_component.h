#pragma once
#include "client/object/component/core/component.h"
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	class Collisioner;
	struct CollisionInfo;
	struct CollisionTreeNode;

	class SceneComponent : public Component, public std::enable_shared_from_this<SceneComponent>
	{
	protected:
		SceneComponent(const std::string& name, int update_order = 100);
		virtual ~SceneComponent();

		virtual bool InitializeComponent() override;
		virtual void ShutdownComponent() override;
		virtual void UpdateComponent(float delta_time) override;

		virtual void UpdateWorldMatrix() override;

	private:
		virtual void UpdateLocalMatrix();

	protected:
		virtual void UpdateOrientedBox();
		void RegisterToCollisionOctree();
		void ReregisterToCollisionOctree();
		void UnregisterToCollsionOctree();

	protected:
		Mat4 m_world_matrix;
		Vec3 m_world_position;
		Vec3 m_world_previous_position;
		Quaternion m_world_rotation;
		Vec3 m_world_scale;
		bool m_is_updated_world_matrix = false;

		Mat4 m_local_matrix;			//local
		Vec3 m_local_position;			//local
		Quaternion m_local_rotation;	//local
		Vec3 m_local_scale;				//local
		bool m_update_local_matrix = true;

	protected:
		SPtr<BOrientedBox> m_oriented_box;
		UPtr<Collisioner> m_collisioner;
		bool m_is_physics = false;
		std::function<void(const SPtr<SceneComponent>&, const SPtr<Actor>&,
			const SPtr<SceneComponent>&)> m_collision_responce_function = nullptr;

	private:
		std::vector<WPtr<CollisionTreeNode>> m_collision_tree_node;
		std::map<std::string, std::set<std::string>> m_collided_components;

	public:
		// �浹 Event�� �߻��� Event�� ����ڰ� ����մϴ�.
		void OnCollisionResponse(const std::function<void(const SPtr<SceneComponent>& comp,
			const SPtr<Actor>& other_actor, const SPtr<SceneComponent>& other_comp)>& function)
		{ m_collision_responce_function = function; }

		// �浹 �˰��򿡼� ���� �浹�� �Ǿ��� �� Event �߻� ���ο� ����(CollisionUtil ����ü�� generate_collision_event�� true���� ȣ��)
		// �浹 ���� �ý��ۿ��� ȣ���ϴ� �Լ��Դϴ�. ����ڰ� ȣ���� �ʿ䰡 �����ϴ�.
		virtual void ExecuteCollisionResponse(const SPtr<SceneComponent>& comp,
			const SPtr<Actor>& other_actor, const SPtr<SceneComponent>& other_comp);

	public:
		//
		//  �� ����ڿ� �Լ� O
		//

		const Mat4& GetWorldMatrix() const { return m_world_matrix; }
		const Vec3& GetWorldPosition() const { return m_world_position; }
		const Vec3& GetWorldPreviousPosition() const { return m_world_previous_position; }
		const Quaternion& GetWorldRotation() const { return m_world_rotation; }
		const Vec3& GetWorldScale() const { return m_world_scale; }
		Vec3 GetWorldForward() const { return vec3::TransformNormal(vec3::AXIS_Z, m_world_rotation); }
		Vec3 GetWorldRight() const { return vec3::TransformNormal(vec3::AXIS_X, m_world_rotation); }
		Vec3 GetWorldUp() const { return vec3::TransformNormal(vec3::AXIS_Y, m_world_rotation); }
		bool IsUpdatedWorldMatrix() const { return m_is_updated_world_matrix; }

		const Vec3& GetLocalPosition() const { return m_local_position; }
		void SetLocalPosition(const Vec3& pos);
		const Quaternion& GetLocalRotation() const { return m_local_rotation; }
		void SetLocalRotation(const Quaternion& rot);
		void SetLocalRotation(float pitch, float yaw, float roll);
		const Vec3& GetScale() const { return m_local_scale; }
		void SetLocalScale(const Vec3& scale);
		void SetLocalScale(float scale);
		Vec3 GetLocalForward() const { return vec3::TransformNormal(vec3::AXIS_Z, m_local_rotation); }
		Vec3 GetLocalRight() const { return vec3::TransformNormal(vec3::AXIS_X, m_local_rotation); }
		Vec3 GetLocalUp() const { return vec3::TransformNormal(vec3::AXIS_Y, m_local_rotation); }

		const SPtr<BOrientedBox>& GetOrientedBox() const { return m_oriented_box; }

		const UPtr<Collisioner>& GetCollisioner() const;
		//SetCollision ���� �Լ��� Actor�� AttachComponent�Լ��� ȣ���� �Ŀ� �ҷ��ּ���.
		//Collision ���� ��Ÿ�� ������ �ڵ��
		void SetNoCollision();
		void SetCollisionInfo(bool is_collision, bool is_blocking, bool generate_collsion_event);
		//Collision ���� ��Ÿ�� �������� ���� �ڵ��
		void SetCollisionInfo(bool is_collision, bool is_blocking, std::string&& collision_type,
			std::set<std::string>&& collisionable_types, bool generate_collision_event);

		bool IsPhysics() const { return m_is_physics; }
		// ���� ����� �����մϴ�. (�߷�)
		void SetPhysics(bool value);

		
	public:
		//
		//  �� ����ڿ� �Լ� X
		//

		// �浹�� �ʿ��� Octree���� � Node�� �ִ��� �߰��ϴ� �Լ��Դϴ�.
		// ���� �� ������ �����ϱ� ���� ����Դϴ�. ����ڰ� ���� ȣ���� �ʿ�� ���� �Լ��Դϴ�. 
		void AddCollisionTreeNode(const WPtr<CollisionTreeNode>& tree_node);
		void ResetCollisionTreeNode() { m_collision_tree_node.clear(); }
		const std::vector<WPtr<CollisionTreeNode>>& GetCollisionTreeNodes() const { return m_collision_tree_node; }

		// �浹 �˰��� �Լ��Դϴ�. Octree�� ����ؼ� �浹 �˰����� �����ϱ� ������
		// ���� �浹�� �ߴ��� ���θ� �Ǵ��մϴ�. ����ڰ� ���� ȣ���� �ʿ�� ���� �Լ��Դϴ�.
		bool IsCollidedComponent(const SPtr<SceneComponent>& component);
		void AddCollidedComponent(const SPtr<SceneComponent>& component);
	};

}


