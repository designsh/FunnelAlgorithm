#pragma once
#include <GameEngine/GameEngineActor.h>

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineRenderer;
class GameEngineCollision;
class TestActor : public GameEngineActor
{
public:

protected:

private:
	void Start() override;
	void Update(float _deltaTime) override;

public:
	TestActor();
	~TestActor();

protected:		// delete constructer
	TestActor(const TestActor& _other) = delete;
	TestActor(TestActor&& _other) noexcept = delete;

private:		//delete operator
	TestActor& operator=(const TestActor& _other) = delete;
	TestActor& operator=(const TestActor&& _other) = delete;

public:
	void SetResultColor(const float4& _Color);

protected:

private:
	GameEngineRenderer* BoxRenderer_;
	GameEngineCollision* BoxCollider_;
};

