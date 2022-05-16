#include "stdafx.h"
#include "client/object/component/mesh/core/mesh_component.h"
#include "client/physics/collision/collision_checker.h"
#include "client/util/octree/octree_manager.h"
#include "client/util/octree/octree.h"
#include "client/physics/collision/collisioner/collisioner.h"

namespace client_fw
{
	CollisionChecker::CollisionChecker()
	{
	}

	void CollisionChecker::CheckCollisions()
	{
		const auto& collision_octrees = CollisionOctreeManager::GetOctreeManager().GetCollisionOctrees();
		for (const auto& octree : collision_octrees)
		{
			CheckCollisionInLeafNode(octree->GetRootNode());
		}
	}

	void CollisionChecker::CheckCollisionInLeafNode(const SPtr<CollisionTreeNode>& node)
	{
		if (node->child_nodes[0] == nullptr)
		{
			if (node->movable_scene_components.empty() == false)
			{
				auto CheckCollision([](const std::string& type, const SPtr<SceneComponent>& comp,
					const SPtr<SceneComponent>& other_comp) {

						const auto& col_info = comp->GetCollisioner()->GetCollisionInfo();
						const auto& other_col_info = other_comp->GetCollisioner()->GetCollisionInfo();

						if (col_info.is_collision == true && other_col_info.is_collision == true &&
							other_col_info.collisionable_types.find(type) != other_col_info.collisionable_types.cend() &&
							comp->GetOwner().lock() != other_comp->GetOwner().lock() &&
							comp->IsCollidedComponent(other_comp) == false &&
							other_comp->IsCollidedComponent(comp) == false)
						{
							if (comp->GetCollisioner()->CheckCollisionWithOtherComponent(other_comp))
							{
								comp->AddCollidedComponent(other_comp);
								other_comp->AddCollidedComponent(comp);

								const auto& col_info = comp->GetCollisioner()->GetCollisionInfo();
								const auto& other_col_info = other_comp->GetCollisioner()->GetCollisionInfo();

								if (col_info.is_blocking == true && other_col_info.is_blocking == true)
									comp->GetCollisioner()->BlockOtherComponent(other_comp);

								if (col_info.generate_collision_event == true)
									comp->ExecuteCollisionResponse(comp, other_comp->GetOwner().lock(), other_comp);
								if (other_col_info.generate_collision_event == true)
									other_comp->ExecuteCollisionResponse(other_comp, comp->GetOwner().lock(), comp);
							}
						}
					});

				for (const auto& [type, mov_comps] : node->movable_scene_components)
				{
					for (auto iter_comp = mov_comps.cbegin(); iter_comp != mov_comps.cend(); ++iter_comp)
					{
						const auto mov_comp = *iter_comp;

						const auto& col_types = mov_comp->GetCollisioner()->GetCollisionInfo().collisionable_types;

						auto col_iter = std::lower_bound(col_types.cbegin(), col_types.cend(), type);

						//colliston type이 같은 component간의 충돌
						if (col_iter != col_types.cend() && *col_iter == type)
						{
							for (auto iter_other_comp = iter_comp; iter_other_comp != mov_comps.cend(); ++iter_other_comp)
							{
								const auto& other_comp = *iter_other_comp;
								CheckCollision(type, mov_comp, other_comp);
							}
							++col_iter;
						}

						//colliston type이 다른 component간의 충돌
						for (col_iter; col_iter != col_types.cend(); ++col_iter)
						{
							for (const auto& other_comp : node->movable_scene_components[*col_iter])
								CheckCollision(type, mov_comp, other_comp);
						}

						for (const auto& other_type : col_types)
						{
							for (const auto& other_comp : node->static_scene_components[other_type])
								CheckCollision(type, mov_comp, other_comp);
						}
					}
				}
			}
		}
		else
		{
			for (const auto& child_node : node->child_nodes)
				CheckCollisionInLeafNode(child_node);
		}
	}
}
