#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineDevice.h"
#include "ThirdParty/Include/DirectXTex/DirectXTex.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineSampler : public GameEngineObjectNameBase
{
public:
	friend class GameEngineSamplerManager;

	void ReCreate();

	void ReCreate(const D3D11_SAMPLER_DESC& _Info);

	GameEngineSampler(); // default constructer 디폴트 생성자
	~GameEngineSampler(); // default destructer 디폴트 소멸자

	inline ID3D11SamplerState** GetSamplerState()
	{
		return &State_;
	}

	D3D11_SAMPLER_DESC Info_;

protected:		// delete constructer
	GameEngineSampler(const GameEngineSampler& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineSampler(GameEngineSampler&& _other) noexcept = delete; // default RValue Copy constructer 디폴트 RValue 복사생성자
	GameEngineSampler& operator=(const GameEngineSampler& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineSampler& operator=(const GameEngineSampler&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

private:		//delete operator
	ID3D11SamplerState* State_;

	void Create(const D3D11_SAMPLER_DESC& _Info);
};

