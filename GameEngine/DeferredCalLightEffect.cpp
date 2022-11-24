#include "PreCompile.h"
#include "DeferredCalLightEffect.h"

DeferredCalLightEffect::DeferredCalLightEffect() 
{
}

DeferredCalLightEffect::~DeferredCalLightEffect() 
{
}

void DeferredCalLightEffect::Initialize() 
{
	Res_.SettingTexture("PositionTex", Target_->GetTexture(1));
	Res_.SettingTexture("NormalTex", Target_->GetTexture(2));
}

void DeferredCalLightEffect::LightDataSetting(CameraComponent* _Caemra)
{
	const LightsData& LightData = _Caemra->GetLightData();
	Res_.SettingConstantBufferLink("LightsData", LightData);
}

void DeferredCalLightEffect::Effect(float _DeltaTime) 
{
	Result_->Clear();
	Result_->Setting(-1);
	Res_.Setting();
	Effect_->Rendering();
	Res_.ReSet();
	Effect_->Reset();
	// 빛타겟을 텍스처로 사용이 가능하다.
	Result_->Reset();
}