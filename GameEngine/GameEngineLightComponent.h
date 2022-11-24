#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineRenderingPipeLine.h"
#include "GameEngineShaderResHelper.h"

enum class LightShapeType
{
	Direction,
	Mesh,
};

struct LightData
{
	float4 ViewLightDir; // 라이트의 포워드 벡터
	float4 ViewNegLightDir; // 라이트의 포워드 -벡터
	float4 ViewLightPosition; // 라이트의 위치
	float4 AmbientLight;

	float4 DiffuseLightColor;
	float4 AmbientLightColor;
	float4 SpacularLightColor;
	float4 SpacularLightPow;

	float4 LightPower; // x는 디퓨즈 라이트의 강도 y는 스펙큘러의 강도 z는 앰비언트의 강도 w는 모든 강도};
};

struct LightsData 
{
	int LightCount;
	LightData Lights[128];
};

// 설명 :
class GameEngineLightComponent : public GameEngineTransformComponent
{
public:
	// constrcuter destructer
	GameEngineLightComponent();
	~GameEngineLightComponent();

	// delete Function
	GameEngineLightComponent(const GameEngineLightComponent& _Other) = delete;
	GameEngineLightComponent(GameEngineLightComponent&& _Other) noexcept = delete;
	GameEngineLightComponent& operator=(const GameEngineLightComponent& _Other) = delete;
	GameEngineLightComponent& operator=(GameEngineLightComponent&& _Other) noexcept = delete;

	const LightData& GetLightData() 
	{
		return LightDataObject;
	}

	void SetDiffusePower(float _Power) 
	{
		LightDataObject.LightPower.x = _Power;
	}

	void SetSpacularLightPow(float _Pow)
	{
		LightDataObject.SpacularLightPow.x = _Pow;
	}

	void SetAmbientPower(float _Power)
	{
		LightDataObject.LightPower.z = _Power;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	LightShapeType ShapeType;
	LightData LightDataObject;
};

