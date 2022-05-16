#pragma once
#include "client/physics/collision/collisioner/collisioner.h"

namespace client_fw
{
	class StaticMesh;
	class KDTree;

	class StaticMeshCollisioner : public Collisioner
	{
	public:
		StaticMeshCollisioner();
		virtual ~StaticMeshCollisioner() = default;

		virtual bool CheckCollisionWithOtherComponent(const SPtr<SceneComponent>& other) override;
		virtual void BlockOtherComponent(const SPtr<SceneComponent>& other) override;

	private:
		//Check static mesh
		bool CheckCollsionWithStaticMesh(const SPtr<StaticMeshComponent>& mesh1, eCollisionComplex complex1,
			const SPtr<StaticMeshComponent>& mesh2, eCollisionComplex complex2);
		bool CheckCollision(const Mat4& mat1, const SPtr<KDTree>& tree1, const SPtr<KDTreeNode>& node1, const SPtr<StaticMeshComponent>& mesh1,
			const Mat4& mat2, const SPtr<KDTree>& tree2, const SPtr<KDTreeNode>& node2, const SPtr<StaticMeshComponent>& mesh2, const BOrientedBox& box2);
		bool CheckCollision(const BOrientedBox& box1, const Mat4& mat2, const SPtr<KDTree>& tree2, const SPtr<KDTreeNode>& node2);
		bool CheckTriangleCollision(const Mat4& mat1, const SPtr<StaticMeshComponent>& mesh1, const SPtr<KDTreeNode>& node1,
			const Mat4& mat2, const SPtr<StaticMeshComponent>& mesh2, const SPtr<KDTreeNode>& node2);

	private:
		UINT m_node_count = 0, m_tri_count = 0;
	};
}
