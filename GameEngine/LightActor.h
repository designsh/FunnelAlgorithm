#pragma once
#include "GameEngineLightComponent.h"
#include "GameEngineActor.h"

// Ό³Έν :
class LightActor : public GameEngineActor
{
public:
	// constrcuter destructer
	LightActor();
	~LightActor();

	// delete Function
	LightActor(const LightActor& _Other) = delete;
	LightActor(LightActor&& _Other) noexcept = delete;
	LightActor& operator=(const LightActor& _Other) = delete;
	LightActor& operator=(LightActor&& _Other) noexcept = delete;

	GameEngineLightComponent* GetLight() 
	{
		return Light_;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	class GameEngineLightComponent* Light_;
};

