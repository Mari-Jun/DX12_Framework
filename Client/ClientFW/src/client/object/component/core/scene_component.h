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
		// 충돌 Event에 발생할 Event를 사용자가 등록합니다.
		void OnCollisionResponse(const std::function<void(const SPtr<SceneComponent>& comp,
			const SPtr<Actor>& other_actor, const SPtr<SceneComponent>& other_comp)>& function)
		{ m_collision_responce_function = function; }

		// 충돌 알고리즘에서 서로 충돌이 되었을 때 Event 발생 여부에 따라(CollisionUtil 구조체의 generate_collision_event가 true여야 호출)
		// 충돌 검출 시스템에서 호출하는 함수입니다. 사용자가 호출할 필요가 없습니다.
		virtual void ExecuteCollisionResponse(const SPtr<SceneComponent>& comp,
			const SPtr<Actor>& other_actor, const SPtr<SceneComponent>& other_comp);

	public:
		//
		//  ↓ 사용자용 함수 O
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
		//SetCollision 관련 함수는 Actor에 AttachComponent함수를 호출한 후에 불러주세요.
		//Collision 변경 런타임 안전한 코드들
		void SetNoCollision();
		void SetCollisionInfo(bool is_collision, bool is_blocking, bool generate_collsion_event);
		//Collision 변경 런타임 안전하지 않은 코드들
		void SetCollisionInfo(bool is_collision, bool is_blocking, std::string&& collision_type,
			std::set<std::string>&& collisionable_types, bool generate_collision_event);

		bool IsPhysics() const { return m_is_physics; }
		// 물리 기능을 설정합니다. (중력)
		void SetPhysics(bool value);

		
	public:
		//
		//  ↓ 사용자용 함수 X
		//

		// 충돌에 필요한 Octree에서 어떤 Node에 있는지 추가하는 함수입니다.
		// 삭제 시 빠르게 삭제하기 위한 기능입니다. 사용자가 따로 호출할 필요는 없는 함수입니다. 
		void AddCollisionTreeNode(const WPtr<CollisionTreeNode>& tree_node);
		void ResetCollisionTreeNode() { m_collision_tree_node.clear(); }
		const std::vector<WPtr<CollisionTreeNode>>& GetCollisionTreeNodes() const { return m_collision_tree_node; }

		// 충돌 알고리즘에 함수입니다. Octree를 사용해서 충돌 알고리즘을 검출하기 때문에
		// 서로 충돌을 했는지 여부를 판단합니다. 사용자가 따로 호출할 필요는 없는 함수입니다.
		bool IsCollidedComponent(const SPtr<SceneComponent>& component);
		void AddCollidedComponent(const SPtr<SceneComponent>& component);
	};

}


