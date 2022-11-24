#pragma once
#include "GameEngineActor.h"

struct ResultColor 
{
public:
	float4 MulColor = float4::ONE;
	float4 PlusColor = float4::ZERO;
};

// Ό³Έν :
class SKySphereActor : public GameEngineActor
{
public:
	// constrcuter destructer
	SKySphereActor();
	~SKySphereActor();

	// delete Function
	SKySphereActor(const SKySphereActor& _Other) = delete;
	SKySphereActor(SKySphereActor&& _Other) noexcept = delete;
	SKySphereActor& operator=(const SKySphereActor& _Other) = delete;
	SKySphereActor& operator=(SKySphereActor&& _Other) noexcept = delete;

	ResultColor Data;


protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	void SetRadius(float _Radius);

	GameEngineRenderer* Renderer;
};

