#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 몬스터
// 용도 : 
// 설명 : 
class GameEngineFBXMesh;
class GameEngineFBXRenderer;
class GameEngineCollision;
class NaviCell;
class Wolf : public GameEngineActor
{
public:
	void Initialize(NaviCell* _CurNaviCell, const float4& _InitPos);

protected:

private:

private:
	void InputKeyStateCheck(float _DeltaTime);

private:
	void Start() override;
	void Update(float _DeltaTime) override;

public:
	Wolf();
	~Wolf();

protected:		// delete constructer
	Wolf(const Wolf& _other) = delete;
	Wolf(Wolf&& _other) noexcept = delete;

private:		//delete operator
	Wolf& operator=(const Wolf& _other) = delete;
	Wolf& operator=(const Wolf&& _other) = delete;

public:
	static Wolf* MainWolf;

protected:

private:
	GameEngineFBXMesh* BaseMesh_;
	GameEngineFBXRenderer* AnimRenderer_;

private:
	GameEngineCollision* BodyCollider_;
	GameEngineCollision* AttackColiider_;

private:
	std::vector<std::string> AnimNameList_;
	int CurAnimationIndex_;

private:
	NaviCell* CurNaviCell_;
	NaviCell* TargetNaviCell_;
	bool MoveStart_;
};

