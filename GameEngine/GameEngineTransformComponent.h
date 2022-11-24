#pragma once
#include "GameEngineComponent.h"
#include "GameEngineTransform.h"
// Ό³Έν :
class GameEngineTransformComponent : public GameEngineComponent
{
	friend GameEngineActor;

public:
	// constrcuter destructer
	GameEngineTransformComponent();
	virtual ~GameEngineTransformComponent() = 0;

	// delete Function
	GameEngineTransformComponent(const GameEngineTransformComponent& _Other) = delete;
	GameEngineTransformComponent(GameEngineTransformComponent&& _Other) noexcept = delete;
	GameEngineTransformComponent& operator=(const GameEngineTransformComponent& _Other) = delete;
	GameEngineTransformComponent& operator=(GameEngineTransformComponent&& _Other) noexcept = delete;

	GameEngineTransform* GetTransform()
	{
		return &Transform_;
	}

public:
	void AttachTransform(GameEngineTransform* _Transform);
	

protected:
	// virtual void InitComponent(GameEngineActor* Actor_);

private:
	GameEngineTransform Transform_;
};

