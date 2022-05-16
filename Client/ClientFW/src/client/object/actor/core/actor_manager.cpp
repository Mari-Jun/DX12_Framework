#include "stdafx.h"
#include "client/object/actor/core/actor_manager.h"
#include "client/object/actor/core/actor.h"

namespace client_fw
{
	void ActorManager::Shutdown()
	{
		for (const auto& actor : m_ready_actors)
			actor->ShutdownActor();
		for (const auto& actor : m_static_actors)
			actor->ShutdownActor();
		for (const auto& actor : m_destructible_actors)
			actor->ShutdownActor();
		for (const auto& actor : m_movable_actors)
			actor->ShutdownActor();
	}

	void ActorManager::Update(float delta_time)
	{
		for (auto& actor : m_ready_actors)
		{
			UINT64 index = ++m_actor_names[actor->GetName()];
			actor->SetName(actor->GetName() + std::to_string(index));

			if (actor->InitializeActor())
			{
				switch (actor->GetMobilityState())
				{
				case eMobilityState::kStatic:
					actor->UpdateActor(delta_time);
					m_static_actors.emplace_back(std::move(actor));
					break;
				case eMobilityState::kDestructible:
					actor->UpdateActor(delta_time);
					m_destructible_actors.emplace_back(std::move(actor));
					break;
				case eMobilityState::kMovable:
					m_movable_actors.emplace_back(std::move(actor));
					break;
				}
			}
			else
			{
				LOG_ERROR("Could not initialize actor : {0}", actor->GetName());
				actor->ShutdownActor();
			}
		}
		m_ready_actors.clear();

		UpdateStaticActors(delta_time);
		UpdateDestructibleActors(delta_time);
		UpdateMovableActors(delta_time);
	}

	void ActorManager::UpdateWorldMatrix()
	{
		for (const auto& actor : m_movable_actors)
			actor->UpdateWorldMatrix();
	}

	void ActorManager::UpdateStaticActors(float delta_time)
	{
		for (const auto& actor : m_static_actors)
		{
			if (actor->GetActorState() == eActorState::kActive && actor->IsUseUpdate())
				actor->UpdateActor(delta_time);
		}
	}

	void ActorManager::UpdateDestructibleActors(float delta_time)
	{
		int count = 0;

		for (auto actor = m_destructible_actors.rbegin(); actor != m_destructible_actors.rend(); ++actor)
		{
			switch ((*actor)->GetActorState())
			{
			case eActorState::kActive:
				if ((*actor)->IsUseUpdate())
					(*actor)->UpdateActor(delta_time);
				break;
			case eActorState::kPaused:
				break;
			case eActorState::kDead:
				(*actor)->ShutdownActor();
				std::iter_swap(actor, m_destructible_actors.rbegin() + count);
				++count;
				break;
			}
		}

		while (count--)
			m_destructible_actors.pop_back();
	}

	void ActorManager::UpdateMovableActors(float delta_time)
	{
		int count = 0;

		for (auto actor = m_movable_actors.rbegin(); actor != m_movable_actors.rend(); ++actor)
		{
			switch ((*actor)->GetActorState())
			{
			case eActorState::kActive:
				(*actor)->UpdateActor(delta_time);
				break;
			case eActorState::kPaused:
				break;
			case eActorState::kDead:
				(*actor)->ShutdownActor();
				std::iter_swap(actor, m_movable_actors.rbegin() + count);
				++count;
				break;
			}
		}

		while (count--)
			m_movable_actors.pop_back();
	}

	void ActorManager::RegisterActor(const SPtr<Actor>& actor)
	{
		m_ready_actors.push_back(actor);
	}
}
