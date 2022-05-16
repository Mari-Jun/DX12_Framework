#pragma once

namespace client_fw
{
	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();
	
		PhysicsWorld(const PhysicsWorld&) = delete;
		PhysicsWorld& operator=(const PhysicsWorld&) = delete;

		bool Initialize();
		void Shutdown();
		void Update(float delta_time);
	};
}



