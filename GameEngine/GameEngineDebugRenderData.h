#pragma once
#include "GameEngineTransform.h"
#include "GameEngineTransformComponent.h"
#include "GameEngineShaderResHelper.h"

// Ό³Έν :
class GameEngineLevel;
class GameEngineRenderingPipeLine;
class GameEngineDebugRenderData
{
	friend class CameraComponent;

public:
	// constrcuter destructer
	GameEngineDebugRenderData();
	~GameEngineDebugRenderData();

	// delete Function
	GameEngineDebugRenderData(const GameEngineDebugRenderData& _Other) 
		: Data_(_Other.Data_)
		, ShaderHelper_(_Other.ShaderHelper_)
		, PipeLine_(_Other.PipeLine_)
	{

	}
	// GameEngineDebugRenderData(GameEngineDebugRenderData&& _Other) noexcept = delete;
	// GameEngineDebugRenderData& operator=(const GameEngineDebugRenderData& _Other) = delete;
	// GameEngineDebugRenderData& operator=(GameEngineDebugRenderData&& _Other) noexcept = delete;

protected:

private:
	TransformData Data_;
	float4 Color_;
	GameEngineShaderResHelper ShaderHelper_;
	GameEngineRenderingPipeLine* PipeLine_;

};

