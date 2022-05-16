#pragma once

namespace client_fw
{
	class BOrientedBox;
	struct CollisionTreeNode;
	enum class eCollisionComplex;

	class CollisionChecker
	{
	public:
		CollisionChecker();

		CollisionChecker(const CollisionChecker&) = delete;
		CollisionChecker& operator=(const CollisionChecker&) = delete;

		static void CheckCollisions();

	private:
		static void CheckCollisionInLeafNode(const SPtr<CollisionTreeNode>& node);
	};
}



