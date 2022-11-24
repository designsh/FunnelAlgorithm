#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

struct RendererData
{
	int IsBump;
	int Shadow;
	int IsAni;
	int Temp2;
	int Temp3;
	int Temp4;
	int Temp5;
	int Temp6;

public:
	RendererData()
		: IsBump(0)
		, Shadow(0)
		, IsAni(0)
		, Temp2(0)
		, Temp3(0)
		, Temp4(0)
		, Temp5(0)
		, Temp6(0)
	{

	}
};

// Ό³Έν :
class CameraComponent;
class GameEngineRendererBase : public GameEngineTransformComponent
{
	friend CameraComponent;

public:
	// constrcuter destructer
	GameEngineRendererBase();
	~GameEngineRendererBase();

	// delete Function
	GameEngineRendererBase(const GameEngineRendererBase& _Other) = delete;
	GameEngineRendererBase(GameEngineRendererBase&& _Other) noexcept = delete;
	GameEngineRendererBase& operator=(const GameEngineRendererBase& _Other) = delete;
	GameEngineRendererBase& operator=(GameEngineRendererBase&& _Other) noexcept = delete;

protected:
	void Start() override;

	virtual void SetRenderGroup(int _Order);

	virtual void Render(float _DeltaTime, bool _IsDeferred);

	RendererData RendererDataInst;
private:

};