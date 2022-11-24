#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>

// Ό³Έν :
class GameEngineLevel;
class GameEngineActor;
class GameEngineComponent : public GameEngineObjectNameBase
{
	friend GameEngineActor;

public:
	// constrcuter destructer
	GameEngineComponent();
	virtual ~GameEngineComponent() = 0;

	// delete Function
	GameEngineComponent(const GameEngineComponent& _Other) = delete;
	GameEngineComponent(GameEngineComponent&& _Other) noexcept = delete;
	GameEngineComponent& operator=(const GameEngineComponent& _Other) = delete;
	GameEngineComponent& operator=(GameEngineComponent&& _Other) noexcept = delete;

	GameEngineActor* GetActor() 
	{
		return Actor_;
	}

	GameEngineLevel* GetLevel();

protected:
	virtual void InitComponent(GameEngineActor* Actor_);

	virtual void Start() = 0;
	virtual void Update(float _DeltaTime) = 0;

private:
	GameEngineActor* Actor_;
};

