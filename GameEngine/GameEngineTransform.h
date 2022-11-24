#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include "GameEngineComponent.h"
#include <GameEngineBase/GameEngineTime.h>

#include <DirectXCollision.h>
#include <DirectXCollision.inl>

struct RayData
{
	float4 Origin;
	float4 Direction;

	RayData()
		: Origin(0.0f, 0.0f, 0.0f)
		, Direction(0.0f, 0.0f, 1.0f)
	{
	}
};

union CollisionData
{
public:
	DirectX::BoundingSphere Sphere;
	DirectX::BoundingBox AABB; // 회전이 고려하면 안되는 박스
	DirectX::BoundingOrientedBox OBB; // 회전한 박스

	CollisionData()
		: OBB()
	{

	}
};

class TransformData
{
public:
	float4 vWorldPosition_;
	float4 vWorldRotation_;
	float4 vWorldScaling_;

	float4 vLocalPosition_;
	float4 vLocalRotation_;
	float4 vLocalScaling_;

	float4x4 LocalScaling_;
	float4x4 LocalRotation_;
	float4x4 LocalPosition_;
	float4x4 LocalWorld_;

	float4x4 Parent_;
	float4x4 WorldWorld_;

	float4x4 View_;
	float4x4 Projection_;

	float4x4 WV_;
	float4x4 WVP_;

public:
	TransformData()
		: vWorldScaling_(float4::ONE)
		, vLocalScaling_(float4::ONE)
	{

	}

	void LocalCalculation()
	{
		LocalScaling_.Scaling(vLocalScaling_);
		LocalRotation_.RotationDeg(vLocalRotation_);
		LocalPosition_.Translation(vLocalPosition_);

		LocalWorld_ = LocalScaling_ * LocalRotation_ * LocalPosition_;
	}

	void ParentSetting(const float4x4& _Parent)
	{
		Parent_ = _Parent;
		WorldWorld_ = LocalWorld_;
		WorldWorld_ *= Parent_;
	}

	void WVPCalculation()
	{
		WV_ = WorldWorld_ * View_;
		WVP_ = WorldWorld_ * View_ * Projection_;
	}

	void WVCalculation()
	{
		WV_ = WorldWorld_ * View_;
	}

	void RootCalculation()
	{
		WorldWorld_ = LocalWorld_;
	}
};

// 기능이란 
// 위치가 존재해야 한다. => 기능

// 어느 위치에 그려져야 한다. => 기능
// 어느 위치에서 다른 애들과 충돌해야 한다 => 기능

// 위치를 나타내는 기능이라 도저히 컴포넌트라고 부를수 없을정도로 중요하다.

// 충돌도 이녀석이 담당할것이기 때문에 어마어마하게 중요하고 잘만들어야 한다.
// 설명 :
class GameEngineTransform
{
public:
	// constrcuter destructer
	GameEngineTransform();
	~GameEngineTransform();

	// delete Function
	//GameEngineTransform(const GameEngineTransform& _Other) = delete;
	//GameEngineTransform(GameEngineTransform&& _Other) noexcept = delete;
	GameEngineTransform& operator=(const GameEngineTransform& _Other) = delete;
	GameEngineTransform& operator=(GameEngineTransform&& _Other) noexcept = delete;

	void TransformUpdate();

	float4 GetLocalScaling() { return TransformData_.vLocalScaling_; }
	float4 GetWorldScaling() { return TransformData_.vWorldScaling_; }
	float4 GetLocalRotation() { return TransformData_.vLocalRotation_; }
	float4 GetWorldRotation() { return TransformData_.vWorldRotation_; }
	float4 GetLocalPosition() { return TransformData_.vLocalPosition_; }
	float4 GetWorldPosition() { return TransformData_.vWorldPosition_; }

	// [1][0][0][0]
	// [0][1][0][0]
	// [0][0][1][0]
	// [0][0][0][1]

	float4 GetWorldForwardVector() { return TransformData_.WorldWorld_.vz.NormalizeReturn3D(); }
	float4 GetLocalForwardVector() { return TransformData_.LocalWorld_.vz.NormalizeReturn3D(); }

	float4 GetWorldBackVector() { return -TransformData_.WorldWorld_.vz.NormalizeReturn3D(); }
	float4 GetLocalBackVector() { return -TransformData_.LocalWorld_.vz.NormalizeReturn3D(); }

	float4 GetWorldRightVector() { return TransformData_.WorldWorld_.vx.NormalizeReturn3D(); }
	float4 GetLocalRightVector() { return TransformData_.LocalWorld_.vx.NormalizeReturn3D(); }

	float4 GetWorldLeftVector() { return -TransformData_.WorldWorld_.vx.NormalizeReturn3D(); }
	float4 GetLocalLeftVector() { return -TransformData_.LocalWorld_.vx.NormalizeReturn3D(); }

	float4 GetWorldUpVector() { return TransformData_.WorldWorld_.vy.NormalizeReturn3D(); }
	float4 GetLocalUpVector() { return TransformData_.LocalWorld_.vy.NormalizeReturn3D(); }

	float4 GetWorldDownVector() { return -TransformData_.WorldWorld_.vy.NormalizeReturn3D(); }
	float4 GetLocalDownVector() { return -TransformData_.LocalWorld_.vy.NormalizeReturn3D(); }

	void SetLocalScaling(const float4& _Value);
	void SetWorldScaling(const float4& _Value);

	void SetLocalRotationDegree(const float4& _Value);
	void SetWorldRotationDegree(const float4& _Value);

	void AddLocalRotationDegreeX(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.x += _Value;
		SetLocalRotationDegree(Local);
	}
	void AddWorldRotationDegreeX(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.x += _Value;
		SetWorldRotationDegree(Local);
	}

	void AddLocalRotationDegreeY(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.y += _Value;
		SetLocalRotationDegree(Local);
	}

	void AddWorldRotationDegreeY(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.y += _Value;
		SetWorldRotationDegree(Local);
	}

	void AddLocalRotationDegreeZ(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.z += _Value;
		SetLocalRotationDegree(Local);
	}
	void AddWorldRotationDegreeZ(const float _Value)
	{
		float4 Local = TransformData_.vLocalRotation_;
		Local.z += _Value;
		SetWorldRotationDegree(Local);
	}

	inline void AddLocalDeltaTimeRotation(const float4& _Value)
	{
		//이현, 임시 수정
		float4 temp = _Value;
		temp.w = 0.f;
		SetLocalRotationDegree(TransformData_.vLocalRotation_ + temp * GameEngineTime::GetInst().GetDeltaTime());
		//SetLocalRotationDegree(TransformData_.vLocalRotation_ + _Value * GameEngineTime::GetInst().GetDeltaTime());
	}

	inline void AddWorldDeltaTimeRotation(const float4& _Value)
	{
		//이현, 임시 수정
		float4 temp = _Value;
		temp.w = 0.f;
		SetWorldRotationDegree(TransformData_.vWorldRotation_ + temp * GameEngineTime::GetInst().GetDeltaTime());
		//SetWorldRotationDegree(TransformData_.vWorldRotation_ + _Value * GameEngineTime::GetInst().GetDeltaTime());
	}

	void SetLocalPosition(const float4& _Value);
	void SetWorldPosition(const float4& _Value);

	inline void SetLocalMove(const float4& _Value)
	{
		float4 temp = _Value;
		temp.w = 0.f;
		SetLocalPosition(TransformData_.vLocalPosition_ + temp);
		//SetLocalPosition(TransformData_.vLocalPosition_ + _Value);
	}

	inline void SetWorldMove(const float4& _Value)
	{
		//이현, 임시 수정
		float4 temp = _Value;
		temp.w = 0.f;
		SetWorldPosition(TransformData_.vWorldPosition_ + temp);
	}

	inline void SetWorldMoveXZ(const float4& _Value)
	{
		//박종원, 추가
		// XY로 들어온 값을 XZ로 변환
		float4 temp = _Value;
		temp.z = temp.y;
		temp.y = 0.f;
		temp.w = 0.f;
		SetWorldPosition(TransformData_.vWorldPosition_ + temp);
	}

	inline void SetLocalDeltaTimeMove(const float4& _Value)
	{
		//이현, 임시 수정
		float4 temp = _Value;
		temp.w = 0.f;
		SetLocalPosition(TransformData_.vLocalPosition_ + temp * GameEngineTime::GetInst().GetDeltaTime());
		//SetLocalPosition(TransformData_.vLocalPosition_ + _Value * GameEngineTime::GetInst().GetDeltaTime());
	}

	inline void SetWorldDeltaTimeMove(const float4& _Value)
	{
		//이현, 임시 수정
		float4 temp = _Value;
		temp.w = 0.f;
		SetWorldPosition(TransformData_.vWorldPosition_ + temp * GameEngineTime::GetInst().GetDeltaTime());
		//SetWorldPosition(TransformData_.vWorldPosition_ + _Value * GameEngineTime::GetInst().GetDeltaTime());
	}

	void DetachChildTransform(GameEngineTransform* _Child);
	void AttachTransform(GameEngineTransform* _Transform);

	inline TransformData& GetTransformData()
	{
		return TransformData_;
	}

	inline const CollisionData& GetCollisionData()
	{
		return ColData_;
	}

	inline const DirectX::BoundingSphere& GetSphere()
	{
		return ColData_.Sphere;
	}

	inline const DirectX::BoundingOrientedBox& GetOBB()
	{
		return ColData_.OBB;
	}

	inline const DirectX::BoundingBox& GetAABB()
	{
		return ColData_.AABB;
	}

	inline float4 GetRayOrigin() const { return RayData_.Origin; }
	inline float4 GetRayDirection() const { return RayData_.Direction; }

	inline void SetRayData(const float4& _origin, const float4& _direction)
	{
		RayData_.Origin = _origin;
		RayData_.Direction = _direction;
	}

	void Copy(const GameEngineTransform& _Other);

protected:
	TransformData TransformData_;
	CollisionData ColData_;
	RayData RayData_;

	GameEngineTransform* Parent_;
	std::vector<GameEngineTransform*> Childs_;


private:
	void AllChildCalculationScaling();
	void AllChildCalculationRotation();
	void AllChildCalculationPosition();

	void CalculationLocalScaling();
	void CalculationWorldScaling();

	void CalculationWorldRotation();
	void CalculationLocalRotation();

	void CalculationLocalPosition();
	void CalculationWorldPosition();
};

