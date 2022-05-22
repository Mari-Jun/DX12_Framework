#include "stdafx.h"
#include "client/object/level/core/level_manager.h"
#include "client/object/level/core/level.h"
#include "client/object/level/core/level_loader.h"
#include "client/object/actor/core/actor.h"
#include "client/util/octree/octree_manager.h"

#include <stdio.h>
#include <time.h>
//#define __USE_LEVEL_CPU_TIME__
//#define __USE_COLLISION_CPU_TIME__

namespace client_fw
{
	LevelManager* LevelManager::s_instance = nullptr;

	LevelManager::LevelManager()
	{
		s_instance = this;
		m_octree_manager = CreateUPtr<OctreeManager>();
	}

	LevelManager::~LevelManager()
	{
	}

	bool LevelManager::Initiallize(const SPtr<LevelSharedInfo>& level_shared_info)
	{
		m_level_shared_info = level_shared_info;
		return true;
	}

	void LevelManager::Shutdown()
	{
		if (m_ready_level != nullptr)
			m_ready_level->ShutdownLevel();
		if (m_cur_level != nullptr)
			m_cur_level->ShutdownLevel();
		m_octree_manager->UnregisterOctrees();
		s_instance = nullptr;
	}

	void LevelManager::Update(float delta_time)
	{
		if (m_dead_level != nullptr)
		{
			m_dead_level->ShutdownLevel();
			m_octree_manager->UnregisterOctrees();
			m_dead_level = nullptr;
		}

		if (m_cur_level != nullptr && m_cur_level->GetLevelState() == eLevelState::kDead)
		{
			m_dead_level = std::move(m_cur_level);
			for (const auto& event : m_level_close_events)
				event();
			m_cur_level = nullptr;
		}

		if (m_ready_level != nullptr && m_dead_level == nullptr)
		{
			m_octree_manager->RegisterVisualOctrees(std::move(m_ready_level->CreateVisualOctrees()));
			m_octree_manager->RegisterCollisionOctrees(std::move(m_ready_level->CreateCollisionOctrees()));
			m_cur_level = std::move(m_ready_level);
			if (m_cur_level->InitializeLevel())
			{
				m_cur_level->SetRuntime();
			}
			else
			{
				LOG_WARN("Could not initailize level : {0}", m_cur_level->GetName());
				m_cur_level->ShutdownLevel();
				m_cur_level = nullptr;
				m_octree_manager->UnregisterOctrees();
			}
			m_ready_level = nullptr;
		}

		if (m_cur_level != nullptr && m_cur_level->GetLevelState() == eLevelState::kInGame)
		{
#ifdef __USE_LEVEL_CPU_TIME__
			clock_t l_start, l_end;
			l_start = clock();
#endif
			m_octree_manager->PrepareUpdate();
			m_cur_level->UpdateLevel(delta_time);

#ifdef __USE_LEVEL_CPU_TIME__
			l_end = clock();
			LOG_TRACE("Level Cpu Time : {0}", float(l_end - l_start));
#endif
	

#ifdef __USE_COLLISION_CPU_TIME__
			clock_t c_start, c_end;
			c_start = clock();
#endif
			m_octree_manager->Update();
#ifdef __USE_COLLISION_CPU_TIME__
			c_end = clock();
			LOG_TRACE("Collision Cpu Time : {0}", float(c_end - c_start));
#endif
		}
	}

	void LevelManager::UpdateWorldMatrix()
	{
		if (m_cur_level != nullptr && m_cur_level->GetLevelState() == eLevelState::kInGame)
		{
			m_cur_level->UpdateWorldMatrix();
		}
	}

	void LevelManager::OpenLevel(const SPtr<Level> new_level, UPtr<LevelLoader>&& level_loader)
	{
		if (m_ready_level != nullptr)
		{
			m_ready_level = nullptr;
		}
		CloseLevel();
		m_ready_level = new_level;
	/*	if (level_loader != nullptr)
		{
			level_loader->LoadLevel(m_ready_level);
		}*/
	}

	void LevelManager::CloseLevel() const
	{
		if(m_cur_level != nullptr)
			m_cur_level->SetLevelState(eLevelState::kDead);
	}

	void LevelManager::SpawnActor(const SPtr<Actor>& actor) const
	{
		if (m_cur_level != nullptr)
			m_cur_level->SpawnActor(actor);
		else
			LOG_WARN("Could not create \"{0}\" : current level is nullptr", actor->GetName());
	}

	void LevelManager::AddLevelCloseEvent(const std::function<void()>& function)
	{
		m_level_close_events.push_back(function);
	}
}
