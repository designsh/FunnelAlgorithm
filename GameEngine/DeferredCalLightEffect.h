#pragma once
#include "GameEnginePostProcessRender.h"

// Ό³Έν :
class DeferredCalLightEffect : public GameEnginePostProcessRender
{
public:
	// constrcuter destructer
	DeferredCalLightEffect();
	~DeferredCalLightEffect();

	// delete Function
	DeferredCalLightEffect(const DeferredCalLightEffect& _Other) = delete;
	DeferredCalLightEffect(DeferredCalLightEffect&& _Other) noexcept = delete;
	DeferredCalLightEffect& operator=(const DeferredCalLightEffect& _Other) = delete;
	DeferredCalLightEffect& operator=(DeferredCalLightEffect&& _Other) noexcept = delete;
	void Effect(float _DeltaTime) override;
	void Initialize() override;

	void LightDataSetting(CameraComponent* Level);

protected:

private:

};

