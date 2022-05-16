#pragma once

namespace client_fw
{
	//���߿� ����� ���Ǹ� �� �� �ְ� ����������Ѵ�.
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

	struct CollisionInfo //��Ÿ�ӿ� ���� �ִ� �����͸� ��ȯ�� �� ������ �߻��� Ȯ���� �ſ� �����ϴ�.
	{
		bool is_collision = false;
		bool is_blocking = false;
		std::string collision_type = "default";
		std::set<std::string> collisionable_types = { "default" };
		eCollisionComplex complex = eCollisionComplex::kLow;
		bool generate_collision_event = false;
	};
}


