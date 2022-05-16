#pragma once

namespace client_fw
{
	class VisualOctree;
	struct VisualTreeNode;
	class CollisionOctree;
	class SceneComponent;
	class RenderComponent;
	class CameraComponent;

	class OctreeManager
	{
	public:
		OctreeManager();
		~OctreeManager() = default;

		OctreeManager(const OctreeManager&) = delete;
		OctreeManager& operator=(const OctreeManager&) = delete;

		void PrepareUpdate();
		void Update();

		void RegisterVisualOctrees(std::vector<SPtr<VisualOctree>>&& octrees);
		void RegisterCollisionOctrees(std::vector<SPtr<CollisionOctree>>&& octrees);
		void UnregisterOctrees();

	private:
		UPtr<class VisualOctreeManager> m_visual_octree_manager;
		UPtr<class CollisionOctreeManager> m_collision_octree_manager;
	};

	class VisualOctreeManager
	{
	public:
		VisualOctreeManager();
		~VisualOctreeManager() = default;

		VisualOctreeManager(const VisualOctreeManager&) = delete;
		VisualOctreeManager& operator=(const VisualOctreeManager&) = delete;

		void PrepareUpdate();
		void ResetLevelOfDetailForShadow(const SPtr<VisualTreeNode>& node);

		void RegisterOctrees(std::vector<SPtr<VisualOctree>>&& octrees);
		void UnregisterOctrees();

		void RegisterRenderComponent(const SPtr<RenderComponent>& render_comp);
		void UnregisterRenderComponent(const SPtr<RenderComponent>& render_comp);

	private:
		static VisualOctreeManager* s_instance;
		bool m_is_active = false;
		std::vector<SPtr<VisualOctree>> m_visual_octrees;
		std::vector<SPtr<RenderComponent>> m_movable_render_comps_for_render;
		std::vector<SPtr<RenderComponent>> m_movable_render_comps_for_shadow;
		std::vector<SPtr<RenderComponent>> m_registered_destructible_render_comps;

	public:
		static VisualOctreeManager& GetOctreeManager() { return *s_instance; }
		const std::vector<SPtr<VisualOctree>>& GetVisualOctrees() const { return m_visual_octrees; }
		const std::vector<SPtr<RenderComponent>>& GetMovableRenderCompsForRender() const { return m_movable_render_comps_for_render; }
		const std::vector<SPtr<RenderComponent>>& GetMovableRenderCompsForShadow() const { return m_movable_render_comps_for_shadow; }
		const std::vector<SPtr<RenderComponent>>& GetRegisteredDestructibleRenderComps() const { return m_registered_destructible_render_comps; }
	};

	class CollisionChecker;

	class CollisionOctreeManager
	{
	public:
		CollisionOctreeManager();
		~CollisionOctreeManager();

		CollisionOctreeManager(const CollisionOctreeManager&) = delete;
		CollisionOctreeManager& operator=(const CollisionOctreeManager&) = delete;

		void Update();

		void RegisterOctrees(std::vector<SPtr<CollisionOctree>>&& octrees);
		void UnregisterOctrees();

		void RegisterSceneComponent(const SPtr<SceneComponent>& scene_comp);
		void UnregisterSceneComponent(const SPtr<SceneComponent>& scene_comp);
		void ReregisterSceneComponent(const SPtr<SceneComponent>& scene_comp);

	private:
		static CollisionOctreeManager* s_instance;
		bool m_is_active = false;
		bool m_checking_collision = false;
		std::vector<SPtr<CollisionOctree>> m_collision_octrees;
		UPtr<CollisionChecker> m_collision_checker;

		std::vector<SPtr<SceneComponent>> m_ready_registered_scene_comp;
		std::vector<SPtr<SceneComponent>> m_ready_unregistered_scene_comp;

	public:
		static CollisionOctreeManager& GetOctreeManager() { return *s_instance; }
		const std::vector<SPtr<CollisionOctree>>& GetCollisionOctrees() const { return m_collision_octrees; }
		void CheckCollsion(bool check) { m_checking_collision = check; }
	};
}


