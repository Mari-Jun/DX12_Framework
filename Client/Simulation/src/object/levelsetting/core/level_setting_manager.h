#pragma once
#include <client/imgui/imgui.h>
#include <client/imgui/imgui_helper.h>

namespace simulation
{
	using namespace client_fw;

	class SimulationLevel;

	template <class Level>
	struct SettingTreeNode
	{
		std::string tree_name;
		std::function<void(const SPtr<Level>&)> node_function;
	};

	template <class Level>
	struct SettingHeaderNode
	{
		std::string header_name;
		std::vector<SettingTreeNode<Level>> tree_nodes;
	};

	template <class Level>
	class LevelNodeManager
	{
	public:
		LevelNodeManager() = default;
		virtual ~LevelNodeManager() = default;

		void ExecuteLevelSettingNodes()
		{
			if (m_owner.expired() == false)
			{
				const auto& level = m_owner.lock();
				for (const auto& header_node : m_level_setting_nodes)
				{
					if (ImGui::CollapsingHeader(header_node.header_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						for (const auto& tree_node : header_node.tree_nodes)
						{
							if (ImGui::TreeNodeEx(tree_node.tree_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
							{
								tree_node.node_function(level);	
								ImGui::Separator();
								ImGui::TreePop();
							}
						}
					}
				}
			}
		}

	protected:
		WPtr<Level> m_owner;
		std::vector<SettingHeaderNode<Level>> m_level_setting_nodes;

	public:
		void SetOwner(const WPtr<Level>& level) { m_owner = level; }

	protected:
		void RegisterSettingHeaderNode(std::string&& header_name, std::vector<SettingTreeNode<Level>>&& setting_tree_nodes)
		{
			m_level_setting_nodes.emplace_back(SettingHeaderNode<Level>{std::move(header_name), std::move(setting_tree_nodes)});
		}
	};
}



