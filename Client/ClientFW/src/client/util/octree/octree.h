#pragma once
#include "client/physics/core/bounding_mesh.h"

namespace client_fw
{
	class SceneComponent;
	class RenderComponent;
	class CameraComponent;

	struct VisualTreeNode
	{
		BBox bounding_box;
		std::vector<SPtr<RenderComponent>> render_components;
		std::array<SPtr<VisualTreeNode>, 8> child_nodes;
	};

	template <class TreeNode>
	class Octree
	{
	public:
		Octree(float width, Vec3 pos, UINT depth)
			: m_width(width), m_position(pos), m_depth(depth)
		{
			if (m_width > 0.0f)
				m_root_node = CreateSPtr<TreeNode>();
		}
		~Octree() = default;

		void Initialize()
		{
			m_root_node->bounding_box = BBox(m_position, Vec3(m_width * 0.5f, m_width * 0.5f, m_width * 0.5f));
			if(m_depth > 0)
				CreateChildNodeInfo(m_root_node, 1);
		}

		void Shutdown()
		{
			m_root_node = nullptr;
		}

	private:
		void CreateChildNodeInfo(const SPtr<TreeNode>& node, UINT depth)
		{
			Vec3 extents = node->bounding_box.GetExtents() * 0.5f;
			Vec3 center = node->bounding_box.GetCenter();

			for (UINT i = 0; i < 8; ++i)
			{
				node->child_nodes[i] = CreateSPtr<TreeNode>();
				node->child_nodes[i]->bounding_box.SetExtents(extents);
				Vec3 new_center;
				new_center.x = ((i % 2 == 0) ? center.x + extents.x : center.x - extents.x);
				new_center.y = ((i / 4 == 0) ? center.y + extents.y : center.y - extents.y);
				new_center.z = ((i % 4 < 2) ? center.z + extents.z : center.z - extents.z);
				node->child_nodes[i]->bounding_box.SetCenter(new_center);
				if (depth < m_depth)
					CreateChildNodeInfo(node->child_nodes[i], depth + 1);
			}
		}

	protected:
		float m_width;
		Vec3 m_position;
		UINT m_depth;
		SPtr<TreeNode> m_root_node;

	public:
		float GetWidth() const { return m_width; }
		const SPtr<TreeNode>& GetRootNode() const { return m_root_node; }
	};

	class VisualOctree : public Octree<VisualTreeNode>
	{
	public:
		VisualOctree(float width, Vec3 pos = vec3::ZERO, UINT depth = 3);

		void RegisterRenderComponent(const SPtr<RenderComponent>& mesh, const SPtr<VisualTreeNode>& node);
	};

	struct CollisionTreeNode
	{
		BBox bounding_box;
		std::map<std::string, std::vector<SPtr<SceneComponent>>> static_scene_components;
		std::map<std::string, std::vector<SPtr<SceneComponent>>> movable_scene_components;
		std::array<SPtr<CollisionTreeNode>, 8> child_nodes;
	};

	class CollisionOctree : public Octree<CollisionTreeNode>
	{
	public:
		CollisionOctree(float width, Vec3 pos = vec3::ZERO, UINT depth = 3);

		void RegisterSceneComponent(const SPtr<SceneComponent>& scene_comp, const SPtr<CollisionTreeNode>& node);
	};
}



