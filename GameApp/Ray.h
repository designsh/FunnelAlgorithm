#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 광선(Ray)
// 용도 : 
// 설명 : 
class GameEngineFBXMesh;
class GameEngineCollision;
class GameEngineFBXRenderer;
class NaviCell;
class Ray : public GameEngineActor
{
public: // 광선체크
	bool IsPicked(const float4& _MousePos, float4& _PickedPos, GameEngineFBXRenderer* _Mesh = nullptr);
	bool IsColliderPicked(const float4& _MousePos, float4& _PickedPos);
	bool IsMeshPicked(GameEngineFBXRenderer* _Mesh, const float4& _MousePos, float4& _PickedPos);

public: // 광선과 NaviCell의 교차
	NaviCell* IsPickedCell(GameEngineFBXRenderer* _Mesh, const float4& _MousePos, float4& _PickedPos);

public: // 광선과 타겟충돌체의 교차
	GameEngineActor* IsPickedTarget(const float4& _MousePos, float4& _PickedPos);

protected:

private:
	bool RayAtViewSpace(float _MousePosX, float _MousePosY);
	bool RayAtViewSpace(float4 _MousePos);

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	Ray();
	~Ray();

protected:		// delete constructer
	Ray(const Ray& _other) = delete;
	Ray(Ray&& _other) noexcept = delete;

private:		//delete operator
	Ray& operator=(const Ray& _other) = delete;
	Ray& operator=(const Ray&& _other) = delete;

public:
	
protected:

private:
	float4 OriginPos_;				// 원점
	float4 Direction_;				// 카메라가 바라보는 방향
};

