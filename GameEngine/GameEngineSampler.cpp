#include "PreCompile.h"
#include "GameEngineSampler.h"
#include "GameEngineBase/GameEngineFile.h"
#include "GameEngineBase/GameEngineString.h"

#pragma comment(lib, "DirectXTex.lib")

GameEngineSampler::GameEngineSampler() // default constructer 디폴트 생성자
	: State_(nullptr)
	, Info_()
{

}

GameEngineSampler::~GameEngineSampler() // default destructer 디폴트 소멸자
{
	if (nullptr != State_)
	{
		State_->Release();
		State_ = nullptr;
	}
}

void GameEngineSampler::ReCreate() 
{
	ReCreate(Info_);
}

void GameEngineSampler::ReCreate(const D3D11_SAMPLER_DESC& _Info) 
{
	if (nullptr != State_)
	{
		State_->Release();
		State_ = nullptr;
	}

	Create(_Info);
}

void GameEngineSampler::Create(const D3D11_SAMPLER_DESC& _Info)
{
	ID3D11SamplerState* State = nullptr;

	if (S_OK != GameEngineDevice::GetDevice()->CreateSamplerState(&_Info, &State))
	{
		GameEngineDebug::MsgBox("샘플러 생성에 실패했습니다.");
		return;
	}

	Info_ = _Info;
	State_ = State;
}