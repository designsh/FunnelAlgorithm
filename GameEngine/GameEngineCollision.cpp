#include "PreCompile.h"
#include "GameEngineCollision.h"
#include "GameEngineLevel.h"
#include "GameEngineTransform.h"

#include "GameEngineFBXMesh.h"


std::function<bool(GameEngineTransform*, GameEngineTransform*)>
GameEngineCollision::CollisionCheckFunction[static_cast<int>(CollisionType::MAX)][static_cast<int>(CollisionType::MAX)];


void GameEngineCollision::Init() 
{
	CollisionCheckFunction[static_cast<int>(CollisionType::CirCle)][static_cast<int>(CollisionType::CirCle)]
		= std::bind(&GameEngineCollision::CirCleToCirCle, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::Sphere3D)][static_cast<int>(CollisionType::Sphere3D)]
		= std::bind(&GameEngineCollision::Sphere3DToSphere3D, std::placeholders::_1, std::placeholders::_2);


	CollisionCheckFunction[static_cast<int>(CollisionType::Rect)][static_cast<int>(CollisionType::Rect)]
		= std::bind(&GameEngineCollision::RectToRect, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::AABBBox3D)][static_cast<int>(CollisionType::AABBBox3D)]
		= std::bind(&GameEngineCollision::AABBToAABB, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::OBBBox3D)][static_cast<int>(CollisionType::OBBBox3D)]
		= std::bind(&GameEngineCollision::OBBToOBB, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::CirCle)][static_cast<int>(CollisionType::AABBBox3D)]
		= std::bind(&GameEngineCollision::CirCleToAABB, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::AABBBox3D)][static_cast<int>(CollisionType::CirCle)]
		= std::bind(&GameEngineCollision::AABBToCirCle, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::Ray)][static_cast<int>(CollisionType::AABBBox3D)]
		= std::bind(&GameEngineCollision::RayToAABB, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::AABBBox3D)][static_cast<int>(CollisionType::Ray)]
		= std::bind(&GameEngineCollision::AABBToRay, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::Ray)][static_cast<int>(CollisionType::OBBBox3D)]
		= std::bind(&GameEngineCollision::RayToOBB, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::OBBBox3D)][static_cast<int>(CollisionType::Ray)]
		= std::bind(&GameEngineCollision::OBBToRay, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::Ray)][static_cast<int>(CollisionType::Sphere3D)]
		= std::bind(&GameEngineCollision::RayToSphere, std::placeholders::_1, std::placeholders::_2);

	CollisionCheckFunction[static_cast<int>(CollisionType::Sphere3D)][static_cast<int>(CollisionType::Ray)]
		= std::bind(&GameEngineCollision::SphereToRay, std::placeholders::_1, std::placeholders::_2);
}


bool GameEngineCollision::OBBToOBB(GameEngineTransform* _Left, GameEngineTransform* _Right) {
	return _Left->GetOBB().Intersects(_Right->GetOBB());
}

bool GameEngineCollision::RectToRect(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingBox Left = _Left->GetAABB();
	DirectX::BoundingBox Right = _Right->GetAABB();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

bool GameEngineCollision::AABBToAABB(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	// DirectX::TriangleTests::Intersects()
	return _Left->GetAABB().Intersects(_Right->GetAABB());
}


bool GameEngineCollision::CirCleToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingSphere Left = _Left->GetSphere();
	DirectX::BoundingSphere Right = _Right->GetSphere();
	Left.Center.z = 0.0f;
	Right.Center.z = 0.0f;
	return Left.Intersects(Right);
}

bool GameEngineCollision::Sphere3DToSphere3D(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	return _Left->GetSphere().Intersects(_Right->GetSphere());
}

bool GameEngineCollision::CirCleToAABB(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	DirectX::BoundingSphere Left = _Left->GetSphere();
	DirectX::BoundingBox Right = _Right->GetAABB();
	Left.Center.y = 0.0f;

	return Left.Intersects(Right);
}

bool GameEngineCollision::AABBToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right)
{
	return CirCleToAABB(_Right, _Left);
}

bool GameEngineCollision::RayToAABB(GameEngineTransform* _left, GameEngineTransform* _right)
{
	DirectX::BoundingBox Right = _right->GetAABB();
	float distance;
	return Right.Intersects(static_cast<DirectX::FXMVECTOR>(_left->GetRayOrigin().DirectVector), static_cast<DirectX::FXMVECTOR>(_left->GetRayDirection().DirectVector), distance);
}

bool GameEngineCollision::AABBToRay(GameEngineTransform* _left, GameEngineTransform* _right)
{
	return RayToAABB(_right, _left);
}

bool GameEngineCollision::RayToOBB(GameEngineTransform* _left, GameEngineTransform* _right)
{
	DirectX::BoundingOrientedBox Right = _right->GetOBB();
	float distance;
	return Right.Intersects(static_cast<DirectX::FXMVECTOR>(_left->GetRayOrigin().DirectVector), static_cast<DirectX::FXMVECTOR>(_left->GetRayDirection().DirectVector), distance);
}

bool GameEngineCollision::OBBToRay(GameEngineTransform* _left, GameEngineTransform* _right)
{
	return RayToOBB(_right, _left);
}

bool GameEngineCollision::RayToSphere(GameEngineTransform* _left, GameEngineTransform* _right)
{
	DirectX::BoundingSphere Right = _right->GetSphere();
	float distance;
	return Right.Intersects(static_cast<DirectX::FXMVECTOR>(_left->GetRayOrigin().DirectVector), static_cast<DirectX::FXMVECTOR>(_left->GetRayDirection().DirectVector), distance);
}

bool GameEngineCollision::SphereToRay(GameEngineTransform* _left, GameEngineTransform* _right)
{
	return RayToSphere(_right, _left);
}

//===================================================== SJH
bool GameEngineCollision::BoundingToRayCollision(CollisionType _ThisType, const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist)
{
	switch (_ThisType)
	{
		case CollisionType::Sphere3D:
		{
			return Sphere3DToRay(_RayOriginPos, _RayDirection, _Dist);
		}
		case CollisionType::AABBBox3D:
		{
			return AABBBoxToRay(_RayOriginPos, _RayDirection, _Dist);
		}
		case CollisionType::OBBBox3D:
		{
			return OBBBoxToRay(_RayOriginPos, _RayDirection, _Dist);
		}
	}

	return false;
}

bool GameEngineCollision::BoundingToRayCollision(const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist)
{
	switch (ColType_)
	{
		case CollisionType::Sphere3D:
		{
			return Sphere3DToRay(_RayOriginPos, _RayDirection, _Dist);
		}
		case CollisionType::AABBBox3D:
		{
			return AABBBoxToRay(_RayOriginPos, _RayDirection, _Dist);
		}
		case CollisionType::OBBBox3D:
		{
			return OBBBoxToRay(_RayOriginPos, _RayDirection, _Dist);
		}
	}

	return false;
}

bool GameEngineCollision::OBBBoxToRay(const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist)
{
	return GetTransform()->GetOBB().Intersects(_RayOriginPos.DirectVector, _RayDirection.DirectVector, _Dist);
}

bool GameEngineCollision::Sphere3DToRay(const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist)
{
	return GetTransform()->GetSphere().Intersects(_RayOriginPos.DirectVector, _RayDirection.DirectVector, _Dist);
}

bool GameEngineCollision::AABBBoxToRay(const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist)
{
	return GetTransform()->GetAABB().Intersects(_RayOriginPos.DirectVector, _RayDirection.DirectVector, _Dist);
}
//===================================================== SJH

GameEngineCollision::GameEngineCollision()
	: ColType_(CollisionType::Rect)
{
}

GameEngineCollision::~GameEngineCollision() 
{
}

void GameEngineCollision::Start()
{
	GetLevel()->PushCollision(this, GetOrder());
}

void GameEngineCollision::Update(float _DeltaTime)
{
	
}

void GameEngineCollision::SetCollisionGroup(int _Type)
{
	GetLevel()->ChangeCollisionGroup(_Type, this);
}

bool GameEngineCollision::Collision(int _OtherGroup)
{
	std::list<GameEngineCollision*>& Group = GetLevel()->GetCollisionGroup(_OtherGroup);

	for (GameEngineCollision* OtherCollision : Group)
	{
		if (false == OtherCollision->IsUpdate())
		{
			continue;
		}

		auto& CheckFunction = CollisionCheckFunction[static_cast<int>(ColType_)][static_cast<int>(OtherCollision->ColType_)];

		if (nullptr == CheckFunction)
		{
			GameEngineDebug::MsgBoxError("아직 구현하지 않는 타입간에 충돌을 하려고 했습니다.");
		}

		return CheckFunction(GetTransform(), OtherCollision->GetTransform());
	}

	return false;
}

GameEngineCollision* GameEngineCollision::CollisionPtr(int _OtherGroup)
{
	if (true == IsUpdate())
	{
		std::list<GameEngineCollision*>& Group = GetLevel()->GetCollisionGroup(_OtherGroup);

		GameEngineCollision* RetCollision = nullptr;
		float retdist = 0.f;
		float comparedist = 0.f;

		for (GameEngineCollision* OtherCollision : Group)
		{
			if (false == OtherCollision->IsUpdate())
			{
				continue;
			}

			auto& CheckFunction = CollisionCheckFunction[static_cast<int>(ColType_)][static_cast<int>(OtherCollision->ColType_)];

			if (nullptr == CheckFunction)
			{
				GameEngineDebug::MsgBoxError("아직 구현하지 않는 타입간에 충돌을 하려고 했습니다.");
			}

			if (false == CheckFunction(GetTransform(), OtherCollision->GetTransform()))
			{
				continue;
			}

			float4 cal = GetTransform()->GetWorldPosition() - OtherCollision->GetTransform()->GetWorldPosition();

			comparedist = cal.Len3D();

			if (retdist == 0.f)
			{
				retdist = comparedist;
				RetCollision = OtherCollision;
			}

			else if (comparedist > retdist)
			{
				continue;
			}
			else
			{
				retdist = comparedist;
				RetCollision = OtherCollision;
			}
		}
		return RetCollision;
	}

	return nullptr;
}

std::list<GameEngineCollision*> GameEngineCollision::CollisionPtrGroup(int _OtherGroup)
{
	std::list<GameEngineCollision*>& Group = GetLevel()->GetCollisionGroup(_OtherGroup);

	std::list<GameEngineCollision*> RetGroup;

	for (GameEngineCollision* OtherCollision : Group)
	{
		if (false == OtherCollision->IsUpdate())
		{
			continue;
		}

		auto& CheckFunction = CollisionCheckFunction[static_cast<int>(ColType_)][static_cast<int>(OtherCollision->ColType_)];

		if (nullptr == CheckFunction)
		{
			GameEngineDebug::MsgBoxError("아직 구현하지 않는 타입간에 충돌을 하려고 했습니다.");
		}

		if (false == CheckFunction(GetTransform(), OtherCollision->GetTransform()))
		{
			continue;
		}

		RetGroup.push_back(OtherCollision);
	}

	return RetGroup;
}

void GameEngineCollision::Collision(CollisionType _ThisType, CollisionType _OtherType, int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack)
{
	std::list<GameEngineCollision*>& Group = GetLevel()->GetCollisionGroup(_OtherGroup);

	for (GameEngineCollision* OtherCollision : Group)
	{
		if (false == OtherCollision->IsUpdate())
		{
			continue;
		}

		auto& CheckFunction = CollisionCheckFunction[static_cast<int>(_ThisType)][static_cast<int>(_OtherType)];

		if (nullptr == CheckFunction)
		{
			GameEngineDebug::MsgBoxError("아직 구현하지 않는 타입간에 충돌을 하려고 했습니다.");
		}

		if (
			false == 
			CheckFunction(GetTransform(), OtherCollision->GetTransform())
			)
		{
			continue;
		}

		_CallBack(OtherCollision);
	}
}

void GameEngineCollision::SphereToSphereCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack) 
{
	Collision(CollisionType::Sphere3D, CollisionType::Sphere3D, _OtherGroup, _CallBack);
}