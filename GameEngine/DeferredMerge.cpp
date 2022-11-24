#include "PreCompile.h"
#include "DeferredMerge.h"

DeferredMerge::DeferredMerge() 
{
}

DeferredMerge::~DeferredMerge() 
{
}


void DeferredMerge::Effect(float _DeltaTime) 
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

void DeferredMerge::SetDeferredTarget(GameEngineRenderTarget* _GBuffer, GameEngineRenderTarget* _Light)
{
	//Texture2D DiffuseTex : register(t0);
	//Texture2D DiffuseLight : register(t1);
	//Texture2D SpacularLight : register(t2);
	//Texture2D AmbientLight : register(t3);


	Res_.SettingTexture("DiffuseTex", _GBuffer->GetTexture(0));
	Res_.SettingTexture("DiffuseLight", _Light->GetTexture(0));
	Res_.SettingTexture("SpacularLight", _Light->GetTexture(1));
	Res_.SettingTexture("AmbientLight", _Light->GetTexture(2));

}

void DeferredMerge::Initialize() 
{

}
