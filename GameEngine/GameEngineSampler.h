#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineDevice.h"
#include "ThirdParty/Include/DirectXTex/DirectXTex.h"

// �з� : 
// �뵵 : 
// ���� : 
class GameEngineSampler : public GameEngineObjectNameBase
{
public:
	friend class GameEngineSamplerManager;

	void ReCreate();

	void ReCreate(const D3D11_SAMPLER_DESC& _Info);

	GameEngineSampler(); // default constructer ����Ʈ ������
	~GameEngineSampler(); // default destructer ����Ʈ �Ҹ���

	inline ID3D11SamplerState** GetSamplerState()
	{
		return &State_;
	}

	D3D11_SAMPLER_DESC Info_;

protected:		// delete constructer
	GameEngineSampler(const GameEngineSampler& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineSampler(GameEngineSampler&& _other) noexcept = delete; // default RValue Copy constructer ����Ʈ RValue ���������
	GameEngineSampler& operator=(const GameEngineSampler& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineSampler& operator=(const GameEngineSampler&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

private:		//delete operator
	ID3D11SamplerState* State_;

	void Create(const D3D11_SAMPLER_DESC& _Info);
};

