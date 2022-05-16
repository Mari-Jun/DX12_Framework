#pragma once

namespace client_fw
{
	//나중에 사용자 정의를 할 수 있게 관리해줘야한다.
	enum class eCollisionPreset
	{
		kNoCollision,
		kBlockAll,
		kOverlapAll,
	};

	enum class eCollisionComplex
	{
		kLow,				//Bounding box(sphere, etc...)
		kHigh,				//KDTree Leaf Triangle Check
	};

	struct CollisionInfo //런타임에 여기 있는 데이터를 변환할 시 문제가 발생할 확률이 매우 높습니다.
	{
		bool is_collision = false;
		bool is_blocking = false;
		std::string collision_type = "default";
		std::set<std::string> collisionable_types = { "default" };
		eCollisionComplex complex = eCollisionComplex::kLow;
		bool generate_collision_event = false;
	};
}


