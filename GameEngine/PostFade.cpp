#include "PreCompile.h"
#include "PostFade.h"


PostFade::PostFade()
{
	Data.Ratio = 1.0f;
	Data.Clip = 0;
}

PostFade::~PostFade()
{
}



void PostFade::Effect(float _DeltaTime) 
{

	// 10

	// ������ 10���ε�
	// 9
	// 10 - 9

	Data.CurTime += _DeltaTime;

	if (Data.CurTime <= 0)
	{
		Data.CurTime = 0.0f;
	}

	//             10          9
	float Inter = Data.CurTime / Data.Time;


	// ��ó���� ���� ��ο�����?
	switch (Option)
	{
	case LIGHT:
		Data.Ratio = Inter;

		if (1 <= Data.Ratio)
		{
			Data.Ratio = 1.0f;
		}

		break;
	case DARK:
		Data.Ratio = 1.0f - Inter;

		if (0 >= Data.Ratio)
		{
			Data.Ratio = 0.0f;
		}

		break;
	default:
		break;
	}

	// ���� ī�޶�Ÿ���� �����ؼ�
	// result�� �׸��� �̴ϴ�.

	Result_->Clear();
	Result_->Setting();
	Res_.SettingTexture("Target", Target_->GetTexture(0));
	Res_.Setting();
	Effect_->Rendering();
	Effect_->Reset();
	Res_.ReSet();

	Target_->Clear(false);
	Target_->Copy(Result_);
}

void PostFade::Initialize() 
{
	CreateResultTarget();
	SetEffect("Fade");
	Res_.SettingConstantBufferLink("FadeData", Data);
	//Data
}

void PostFade::SetData(float _Time, FadeOption _Option)
{
	Data.Ratio = 1.0f;

	Data.Time = _Time;
	Data.CurTime = 0.0f;
	Data.Clip = 0;

	Option = _Option;

}