#include "stdafx.h"
#include "client/asset/mesh/mesh.h"
#include "client/asset/primitive/vertex.h"
#include "client/physics/collision/collisioner/static_mesh_collisioner.h"
#include "client/object/component/mesh/static_mesh_component.h"
#include "client/object/actor/core/actor.h"
#include "client/physics/collision/collision_responser.h"

namespace client_fw
{
	StaticMeshCollisioner::StaticMeshCollisioner()
		: Collisioner(eMeshCollisionType::kStaticMesh)
	{
	}

	bool StaticMeshCollisioner::CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other)
	{
		switch (other->GetCollisioner()->GetMeshCollisionType())
		{
		case eMeshCollisionType::kStaticMesh:
		{
			const auto& mesh1 = std::static_pointer_cast<StaticMeshComponent>(GetOwner());
			const auto& mesh2 = std::static_pointer_cast<StaticMeshComponent>(other->GetCollisioner()->GetOwner());
			return CheckCollsionWithStaticMesh(mesh1, mesh1->GetCollisioner()->GetCollisionInfo().complex,
				mesh2, mesh2->GetCollisioner()->GetCollisionInfo().complex);
		}
		case eMeshCollisionType::kBox:
			return GetOwner()->GetOrientedBox()->Intersects(*other->GetOrientedBox());
		case eMeshCollisionType::kSphere:
		{
			BSphere sphere2(other->GetWorldPosition(), other->GetOrientedBox()->GetExtents().x);
			return GetOwner()->GetOrientedBox()->Intersects(sphere2);
		}
		case eMeshCollisionType::kRay:
			return other->GetCollisioner()->CheckCollisionWithOtherComponent(GetOwner());
		default:
			break;
		}
		return false;
	}

	void StaticMeshCollisioner::BlockOtherComponent(const SPtr<SceneComponent>& other)
	{
		if (other->GetOwner().lock()->GetMobilityState() == eMobilityState::kMovable)
		{
		}
		else
		{
			switch (other->GetCollisioner()->GetMeshCollisionType())
			{
			case eMeshCollisionType::kStaticMesh:
			case eMeshCollisionType::kBox:
				CollisionResponser::BlockBetweenBoxAndBox(GetOwner(), *GetOwner()->GetOrientedBox(), *other->GetOrientedBox());
				break;
			case eMeshCollisionType::kSphere:
			{
				BSphere sphere(other->GetWorldPosition(), other->GetOrientedBox()->GetExtents().x);
				CollisionResponser::BlockBetweenBoxAndSphere(GetOwner(), *GetOwner()->GetOrientedBox(), sphere);
				break;
			}
			default:
				break;
			}
		}
	}

	bool StaticMeshCollisioner::CheckCollsionWithStaticMesh(const SPtr<StaticMeshComponent>& mesh1, eCollisionComplex complex1,
		const SPtr<StaticMeshComponent>& mesh2, eCollisionComplex complex2)
	{
		return mesh1->GetOrientedBox()->Intersects(*mesh2->GetOrientedBox());

		{			
			//const auto& tree1 = mesh1->GetStaticMesh()->GetBoundingTree();
			//const auto& tree2 = mesh2->GetStaticMesh()->GetBoundingTree();

			//m_node_count = 0;
			//m_tri_count = 0;
			//switch (complex1)
			//{
			//case eCollisionComplex::kHigh:
			//{
			//	switch (complex2)
			//	{
			//	case eCollisionComplex::kHigh:
			//	{
			//		BOrientedBox box2;
			//		box2.Transform(tree2->GetRootNode()->box, mesh2->GetWorldMatrix());

			//		if (CheckCollision(mesh1->GetWorldMatrix(), tree1, tree1->GetRootNode(), mesh1,
			//			mesh2->GetWorldMatrix(), tree2, tree2->GetRootNode(), mesh2, box2))
			//		{
			//			//LOG_INFO("{0} col {1}", mesh1->GetOwner().lock()->GetName(), mesh2->GetOwner().lock()->GetName());
			//		}
			//		break;
			//	}
			//	case eCollisionComplex::kLow:
			//	{
			//		if (CheckCollision(*mesh2->GetOrientedBox(), mesh1->GetWorldMatrix(), tree1, tree1->GetRootNode()))
			//		{
			//			//LOG_INFO("{0} col {1}", mesh1->GetOwner().lock()->GetName(), mesh2->GetOwner().lock()->GetName());
			//		}
			//		break;
			//	}
			//	}
			//	break;
			//}
			//case eCollisionComplex::kLow:
			//{
			//	switch (complex2)
			//	{
			//	case eCollisionComplex::kHigh:
			//	{
			//		if (CheckCollision(*mesh1->GetOrientedBox(), mesh2->GetWorldMatrix(), tree2, tree2->GetRootNode()))
			//		{
			//			//LOG_INFO("{0} col {1}", mesh1->GetOwner().lock()->GetName(), mesh2->GetOwner().lock()->GetName());
			//		}
			//		break;
			//	}
			//	case eCollisionComplex::kLow:
			//	{
			//		//LOG_INFO("{0} col {1}", mesh1->GetOwner().lock()->GetName(), mesh2->GetOwner().lock()->GetName());
			//		break;
			//	}
			//	}
			//	break;
			//}
			//}
			//LOG_INFO("충돌 검출 수 : {0} / {1}", m_node_count, m_tri_count);
		}
	}

	bool StaticMeshCollisioner::CheckCollision(const Mat4& mat1, const SPtr<KDTree>& tree1, const SPtr<KDTreeNode>& node1, const SPtr<StaticMeshComponent>& mesh1,
		const Mat4& mat2, const SPtr<KDTree>& tree2, const SPtr<KDTreeNode>& node2, const SPtr<StaticMeshComponent>& mesh2, const BOrientedBox& box2)
	{
		++m_node_count;
		BOrientedBox box1;
		box1.Transform(node1->box, mat1);

		ContainmentType type = box2.Contains(box1);
		switch (type)
		{
		case DirectX::DISJOINT:	return false;
		case DirectX::INTERSECTS:
			if (node1->child0_id == -1)
			{
				if (node2->child0_id == -1)
					return CheckTriangleCollision(mat2, mesh2, node2, mat1, mesh1, node1);
				else
				{
					if (CheckCollision(mat2, tree2, tree2->GetNode(node2->child0_id), mesh2, mat1, tree1, node1, mesh1, box1))
						return true;
					if (CheckCollision(mat2, tree2, tree2->GetNode(node2->child1_id), mesh2, mat1, tree1, node1, mesh1, box1))
						return true;
				}
			}
			else
			{
				if (CheckCollision(mat1, tree1, tree1->GetNode(node1->child0_id), mesh1, mat2, tree2, node2, mesh2, box2))
					return true;
				if (CheckCollision(mat1, tree1, tree1->GetNode(node1->child1_id), mesh1, mat2, tree2, node2, mesh2, box2))
					return true;
			}
			break;
		case DirectX::CONTAINS:
			if (node2->child0_id == -1)
			{
				//B가 A를 포함하는데 B가 leaf node인 경우는 이미 더이상 탐색 할 필요가 없다.
				return true;
			}
			else
			{
				if (CheckCollision(mat2, tree2, tree2->GetNode(node2->child0_id), mesh2, mat1, tree1, node1, mesh1, box1))
					return true;
				if (CheckCollision(mat2, tree2, tree2->GetNode(node2->child1_id), mesh2, mat1, tree1, node1, mesh1, box1))
					return true;
			}
			break;
		}

		return false;
	}

	bool StaticMeshCollisioner::CheckCollision(const BOrientedBox& box1, const Mat4& mat2, const SPtr<KDTree>& tree2, const SPtr<KDTreeNode>& node2)
	{
		return false;
	}

	bool StaticMeshCollisioner::CheckTriangleCollision(const Mat4& mat1, const SPtr<StaticMeshComponent>& mesh1, const SPtr<KDTreeNode>& node1, const Mat4& mat2,
		const SPtr<StaticMeshComponent>& mesh2, const SPtr<KDTreeNode>& node2)
	{
		if (node1->triangle_indices.empty() || node2->triangle_indices.empty())
		{
			return false;
		}
		auto vertices1 = mesh1->GetStaticMesh()->GetVertices(0);
		auto vertices2 = mesh2->GetStaticMesh()->GetVertices(0);

		for (size_t i1 : node1->triangle_indices)
		{
			Vec3 v1(vertices1[i1 * 3 + 0].GetPosition());
			Vec3 v2(vertices1[i1 * 3 + 1].GetPosition());
			Vec3 v3(vertices1[i1 * 3 + 2].GetPosition());
			v1.TransformCoord(mat1), v2.TransformCoord(mat1), v3.TransformCoord(mat1);

			for (size_t i2 : node2->triangle_indices)
			{
				Vec3 v4(vertices2[i2 * 3 + 0].GetPosition());
				Vec3 v5(vertices2[i2 * 3 + 1].GetPosition());
				Vec3 v6(vertices2[i2 * 3 + 2].GetPosition());
				v4.TransformCoord(mat2), v5.TransformCoord(mat2), v6.TransformCoord(mat2);

				++m_tri_count;
				if (triangle_test::Intersect(v1, v2, v3, v4, v5, v6))
				{
					return true;
				}
			}
		}
		return false;
	}
}