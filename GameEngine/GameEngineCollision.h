#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineShaderResHelper.h"
#include "Enums.h"

// 이녀석들만의 그룹을 만들겠습니다.


// 설명 :
class GameEngineCore;
class GameEngineLevel;
class GameEngineFBXMesh;
class GameEngineCollision : public GameEngineTransformComponent
{
	friend GameEngineCore;
	friend GameEngineLevel;

	static std::function<bool(GameEngineTransform*, GameEngineTransform*)> 
		CollisionCheckFunction[static_cast<int>(CollisionType::MAX)][static_cast<int>(CollisionType::MAX)];

	static void Init();

public:
	static bool CirCleToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool Sphere3DToSphere3D(GameEngineTransform* _Left, GameEngineTransform* _Right);

	static bool RectToRect(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool AABBToAABB(GameEngineTransform* _Left, GameEngineTransform* _Right);

	static bool OBBToOBB(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool CirCleToAABB(GameEngineTransform* _Left, GameEngineTransform* _Right);
	static bool AABBToCirCle(GameEngineTransform* _Left, GameEngineTransform* _Right);

	static bool RayToAABB(GameEngineTransform* _left, GameEngineTransform* _right);
	static bool AABBToRay(GameEngineTransform* _left, GameEngineTransform* _right);

	static bool RayToOBB(GameEngineTransform* _left, GameEngineTransform* _right);
	static bool OBBToRay(GameEngineTransform* _left, GameEngineTransform* _right);

	static bool RayToSphere(GameEngineTransform* _left, GameEngineTransform* _right);
	static bool SphereToRay(GameEngineTransform* _left, GameEngineTransform* _right);

//======================================== SJH
private: // 타입별 광선과의 교차검사
	bool OBBBoxToRay(const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist);
	bool Sphere3DToRay(const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist);
	bool AABBBoxToRay(const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist);

public: // 바운딩박스,구를 이용한 광선과의 충돌체크
	bool BoundingToRayCollision(CollisionType _ThisType, const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist);
	bool BoundingToRayCollision(const float4& _RayOriginPos, const float4& _RayDirection, float& _Dist);

public:
	inline CollisionType GetCollisionType()
	{
		return ColType_;
	}
//======================================== SJH

public:
	inline void SetRayData(const float4& _origin, const float4& _direction) { GetTransform()->SetRayData(_origin, _direction); }

public:
	// constrcuter destructer
	GameEngineCollision();
	~GameEngineCollision();

	// delete Function
	GameEngineCollision(const GameEngineCollision& _Other) = delete;
	GameEngineCollision(GameEngineCollision&& _Other) noexcept = delete;
	GameEngineCollision& operator=(const GameEngineCollision& _Other) = delete;
	GameEngineCollision& operator=(GameEngineCollision&& _Other) noexcept = delete;

	template<typename UserType>
	void SetCollisionGroup(UserType _Type)
	{
		SetCollisionGroup(static_cast<int>(_Type));
	}

	void SetCollisionGroup(int _Type);

	void Collision(
		CollisionType _ThisType, 
		CollisionType _OtherType, 
		int _OtherGroup, 
		std::function<void(GameEngineCollision*)> _CallBack
	);


	void SphereToSphereCollision(int _OtherGroup, std::function<void(GameEngineCollision*)> _CallBack);

	/// 이현 ///
	void SetCollisionType(CollisionType _ColType)
	{
		ColType_ = _ColType;
	}

	void SetCollisionInfo(int _Type, CollisionType _ColType)
	{
		SetCollisionGroup(_Type);
		SetCollisionType(_ColType);
	}

	//충돌 여부를 Bool 값으로 리턴
	bool Collision(int _OtherGroup);
	template <typename T> bool Collision(T _OtherGroup);;
	//충돌한 가장 가까운 ptr 리턴
	GameEngineCollision* CollisionPtr(int _OtherGroup);
	//충돌한 새끼들 싹다 리스트로 리턴
	std::list<GameEngineCollision*> CollisionPtrGroup(int _OtherGroup);


	// 박종원 0728
	// 콜리젼에게 "소속" 을 등록해서
	// 캐릭터 등 액터에서 만들어주는 콜리젼에게 일괄적인 소속을 부여함
	// 콜리젼 체크 대상이지만, 같은 소속에 들어 있는 콜리젼이 탐지될 경우 패스하는 시스템임.
	// 왜 필요한가? 
	// 캐릭터 각각이 캐릭터 콜리젼, 공격 범위 콜리젼을 가지고 있고,
	// 공격 범위 콜리젼 내에 캐릭터 콜리젼이 있으면 공격을 시도할 수 있을 때,
	// 자기 캐릭터 콜리젼을 자기 공격 범위 콜리젼이 인식하지 않기 위해...


protected:
	CollisionType ColType_;
	///  이현 ///

private:
	void Start() override;
	void Update(float _DeltaTime) override;
};

template<typename T>
inline bool GameEngineCollision::Collision(T _OtherGroup) { return Collision(static_cast<int>(_OtherGroup)); }
