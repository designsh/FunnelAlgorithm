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
	float4 ViewLightDir; // ����Ʈ�� ������ ����
	float4 ViewNegLightDir; // ����Ʈ�� ������ -����
	float4 ViewLightPosition; // ����Ʈ�� ��ġ
	float4 AmbientLight;

	float4 DiffuseLightColor;
	float4 AmbientLightColor;
	float4 SpacularLightColor;
	float4 SpacularLightPow;

	float4 LightPower; // x�� ��ǻ�� ����Ʈ�� ���� y�� ����ŧ���� ���� z�� �ں��Ʈ�� ���� w�� ��� ����};
};

struct LightsData 
{
	int LightCount;
	LightData Lights[128];
};

// ���� :
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

