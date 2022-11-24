#pragma once
#include "GameEnginePostProcessRender.h"

enum FadeOption
{
	LIGHT,
	DARK,
};

struct FadeData
{
	float CurTime;// 1
	float Ratio; // 
	int Clip; // 1 ������ 0�� �ֵ��� ���Ѵ�.
			  // 0 ������ 0�� �ֵ��� �Ѵ�.

	float Time;

	FadeData()
		: Clip(1)
		, Ratio(0.0f)
		, CurTime(0.0f)
		, Time(0.0f)
	{

	}
};

// ���� :
class PostFade : public GameEnginePostProcessRender
{
public:
	// constrcuter destructer
	PostFade();
	~PostFade();

	// delete Function
	PostFade(const PostFade& _Other) = delete;
	PostFade(PostFade&& _Other) noexcept = delete;
	PostFade& operator=(const PostFade& _Other) = delete;
	PostFade& operator=(PostFade&& _Other) noexcept = delete;

	void SetData(float _Time, FadeOption _Option);
protected:
	void Effect(float _DeltaTime) override;
	void Initialize() override;


private:
	FadeData Data;
	FadeOption Option;


};

