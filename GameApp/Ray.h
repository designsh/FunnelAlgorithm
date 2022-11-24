#pragma once
#include <GameEngine/GameEngineActor.h>

// �з� : ����(Ray)
// �뵵 : 
// ���� : 
class GameEngineFBXMesh;
class GameEngineCollision;
class GameEngineFBXRenderer;
class NaviCell;
class Ray : public GameEngineActor
{
public: // ����üũ
	bool IsPicked(const float4& _MousePos, float4& _PickedPos, GameEngineFBXRenderer* _Mesh = nullptr);
	bool IsColliderPicked(const float4& _MousePos, float4& _PickedPos);
	bool IsMeshPicked(GameEngineFBXRenderer* _Mesh, const float4& _MousePos, float4& _PickedPos);

public: // ������ NaviCell�� ����
	NaviCell* IsPickedCell(GameEngineFBXRenderer* _Mesh, const float4& _MousePos, float4& _PickedPos);

public: // ������ Ÿ���浹ü�� ����
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
	float4 OriginPos_;				// ����
	float4 Direction_;				// ī�޶� �ٶ󺸴� ����
};

