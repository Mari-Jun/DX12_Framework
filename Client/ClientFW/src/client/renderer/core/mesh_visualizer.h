#pragma once

namespace DirectX
{
	enum ContainmentType;
}

namespace client_fw
{
	template <class Type>
	class Bounding;
	class BFrustum;

	class CameraComponent;
	class RenderComponent;
	struct VisualTreeNode;

	class MeshVisualizer final
	{
	private:
		template <class Type>
		static void UpdateVisibilityFromVisualTreeNode(const Bounding<Type>& bounding,
			ContainmentType type, const SPtr<VisualTreeNode>& node, const Vec3& eye,
			const std::function<bool(const SPtr<RenderComponent>&)>& trigger_function,
			const std::function<void(const SPtr<RenderComponent>&, const Vec3&)>& execute_function)
		{
			if (node->child_nodes[0] == nullptr)
			{
				switch (type)
				{
				case DirectX::INTERSECTS:
					for (const auto& render_cmp : node->render_components)
					{
						if (trigger_function(render_cmp))
						{
							if (bounding.Intersects(*render_cmp->GetOrientedBox()))
							{
								execute_function(render_cmp, eye);
							}
						}
					}
					break;
				case DirectX::CONTAINS:
					for (const auto& render_cmp : node->render_components)
					{
						if (trigger_function(render_cmp))
						{
							execute_function(render_cmp, eye);
						}
					}
					break;
				default:
					break;
				}

				return;
			}

			switch (type)
			{
			case DirectX::INTERSECTS:
			{
				ContainmentType type;
				for (UINT i = 0; i < 8; ++i)
				{
					type = bounding.Contains(node->child_nodes[i]->bounding_box);
					if (type != ContainmentType::DISJOINT)
						UpdateVisibilityFromVisualTreeNode(bounding, type, node->child_nodes[i], eye, trigger_function, execute_function);
				}
				break;
			}
			case DirectX::CONTAINS:
				for (UINT i = 0; i < 8; ++i)
					UpdateVisibilityFromVisualTreeNode(bounding, type, node->child_nodes[i], eye, trigger_function, execute_function);
				break;
			default:
				break;
			}
		}

		template <class Type>
		static void AddDestructibleFromVisualTreeNodeToCamera(const SPtr<CameraComponent>& camera_comp,
			const Bounding<Type>& bounding,	ContainmentType type, const SPtr<VisualTreeNode>& node, const Vec3& eye,
			const std::function<bool(const SPtr<RenderComponent>&)>& trigger_function)
		{
			if (node->child_nodes[0] == nullptr)
			{
				switch (type)
				{
				case DirectX::INTERSECTS:
					for (const auto& render_cmp : node->render_components)
					{
						if (trigger_function(render_cmp))
						{
							if (bounding.Intersects(*render_cmp->GetOrientedBox()))
								camera_comp->RegisterDestructibleRenderComponent(render_cmp);
						}
					}
					break;
				case DirectX::CONTAINS:
					for (const auto& render_cmp : node->render_components)
					{
						if (trigger_function(render_cmp))
							camera_comp->RegisterDestructibleRenderComponent(render_cmp);
					}
					break;
				default:
					break;
				}

				return;
			}

			switch (type)
			{
			case DirectX::INTERSECTS:
			{
				ContainmentType type;
				for (UINT i = 0; i < 8; ++i)
				{
					type = bounding.Contains(node->child_nodes[i]->bounding_box);
					if (type != ContainmentType::DISJOINT)
						AddDestructibleFromVisualTreeNodeToCamera(camera_comp, bounding, type, node->child_nodes[i], eye, trigger_function);
				}
				break;
			}
			case DirectX::CONTAINS:
				for (UINT i = 0; i < 8; ++i)
					AddDestructibleFromVisualTreeNodeToCamera(camera_comp, bounding, type, node->child_nodes[i], eye, trigger_function);
				break;
			default:
				break;
			}
		}

		template <class Type>
		static void AddDestructibleFromVisualTreeNodeToCamera(const SPtr<CameraComponent>& camera_comp, 
			const Bounding<Type>& bounding, const Vec3& eye,
			const std::function<bool(const SPtr<RenderComponent>&)>& trigger_function)
		{
			const auto& visual_octrees = VisualOctreeManager::GetOctreeManager().GetVisualOctrees();
			for (const auto& octree : visual_octrees)
			{
				const auto& root_node = octree->GetRootNode();

				ContainmentType type = bounding.Contains(root_node->bounding_box);
				if (type != ContainmentType::DISJOINT)
				{
					AddDestructibleFromVisualTreeNodeToCamera(camera_comp, bounding, type, root_node, eye, trigger_function);
				}
			}
		}

		template <class Type>
		static void UpdateVisibilityForStatic(const Bounding<Type>& bounding, const Vec3& eye,
			const std::function<bool(const SPtr<RenderComponent>&)>& trigger_function,
			const std::function<void(const SPtr<RenderComponent>&, const Vec3&)>& execute_function)
		{
			const auto& visual_octrees = VisualOctreeManager::GetOctreeManager().GetVisualOctrees();
			for (const auto& octree : visual_octrees)
			{
				const auto& root_node = octree->GetRootNode();

				ContainmentType type = bounding.Contains(root_node->bounding_box);
				if (type != ContainmentType::DISJOINT)
				{
					UpdateVisibilityFromVisualTreeNode(bounding, type, root_node, eye, trigger_function, execute_function);
				}
			}
		}

		template <class Type>
		static void AddRegisteredDestructibleToCamera(const SPtr<CameraComponent>& camera_comp, 
			const Bounding<Type>& bounding, const std::function<bool(const SPtr<RenderComponent>&)>& trigger_function)
		{
			const auto& destructible_render_comps = VisualOctreeManager::GetOctreeManager().GetRegisteredDestructibleRenderComps();

			for (const auto& render_comp : destructible_render_comps)
			{
				if (trigger_function(render_comp) &&
					bounding.Intersects(*render_comp->GetOrientedBox()))
				{
					camera_comp->RegisterDestructibleRenderComponent(render_comp);
				}
			}
		}

		template <class Type>
		static void UpdateVisibilityMovableForRender(const Bounding<Type>& bounding, const Vec3& eye,
			const std::function<bool(const SPtr<RenderComponent>&)>& trigger_function,
			const std::function<void(const SPtr<RenderComponent>&, const Vec3&)>& execute_function)
		{
			const auto& movable_render_comps = VisualOctreeManager::GetOctreeManager().GetMovableRenderCompsForRender();

			for (const auto& render_comp : movable_render_comps)
			{
				if (trigger_function(render_comp) &&
					bounding.Intersects(*render_comp->GetOrientedBox()))
				{
					execute_function(render_comp, eye);
				}
			}
		}

		template <class Type>
		static void UpdateVisibilityMovableForShadow(const Bounding<Type>& bounding, const Vec3& eye,
			const std::function<bool(const SPtr<RenderComponent>&)>& trigger_function,
			const std::function<void(const SPtr<RenderComponent>&, const Vec3&)>& execute_function)
		{
			const auto& movable_render_comps = VisualOctreeManager::GetOctreeManager().GetMovableRenderCompsForShadow();

			for (const auto& render_comp : movable_render_comps)
			{
				if (trigger_function(render_comp) &&
					bounding.Intersects(*render_comp->GetOrientedBox()))
				{
					execute_function(render_comp, eye);
				}
			}
		}

		template <class Type>
		static void UpdateVisibilityForRender(const Bounding<Type>& bounding, const Vec3& eye,
			const std::function<bool(const SPtr<RenderComponent>&)>& trigger_function,
			const std::function<void(const SPtr<RenderComponent>&, const Vec3&)>& execute_function)
		{
			UpdateVisibilityForStatic(bounding, eye, trigger_function, execute_function);
			UpdateVisibilityMovableForRender(bounding, eye, trigger_function, execute_function);
		}

		template <class Type>
		static void UpdateVisibilityForShadow(const Bounding<Type>& bounding, const Vec3& eye,
			const std::function<bool(const SPtr<RenderComponent>&)>& trigger_function,
			const std::function<void(const SPtr<RenderComponent>&, const Vec3&)>& execute_function)
		{
			UpdateVisibilityForStatic(bounding, eye, trigger_function, execute_function);
			UpdateVisibilityMovableForShadow(bounding, eye, trigger_function, execute_function);
		}

	public:
		static void UpdateVisibilityFromMovableRenderCamera(const SPtr<CameraComponent>& camera);
		static void UpdateVisibilityFromMovableShadowCamera(const SPtr<CameraComponent>& camera);
		static void UpdateVisibilityFromMovableShadowSphere(const SPtr<CameraComponent>& camera, const Vec3& eye, float radius);

		static void UpdateVisibilityFromStaticShadowCamera(const SPtr<CameraComponent>& camera, bool update_static_render_comp = false);
		static void UpdateVisibilityFromStaticShadowSphere(const SPtr<CameraComponent>& camera, const Vec3& eye, float radius, bool update_static_render_comp = false);
	};
}



