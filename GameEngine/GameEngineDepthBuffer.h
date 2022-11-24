#pragma once
#include "GameEngineTexture.h"

// Ό³Έν :
class GameEngineDepthBuffer : public GameEngineObjectNameBase
{
public:
	// constrcuter destructer
	GameEngineDepthBuffer();
	~GameEngineDepthBuffer();

	// delete Function
	GameEngineDepthBuffer(const GameEngineDepthBuffer& _Other) = delete;
	GameEngineDepthBuffer(GameEngineDepthBuffer&& _Other) noexcept = delete;
	GameEngineDepthBuffer& operator=(const GameEngineDepthBuffer& _Other) = delete;
	GameEngineDepthBuffer& operator=(GameEngineDepthBuffer&& _Other) noexcept = delete;

	void Create(float4 _Scale);


	inline ID3D11DepthStencilView* GetDepthStencilView()
	{
		return DepthTexture_->GetDepthStencilView();
	}

protected:

private:
	GameEngineTexture* DepthTexture_;
};

