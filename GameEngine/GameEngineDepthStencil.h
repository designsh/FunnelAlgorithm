#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include <GameEngine/GameEngineDevice.h>

// Ό³Έν :
class GameEngineDepthStencil : public GameEngineObjectNameBase
{
public:
	friend class GameEngineDepthStencilManager;


	// constrcuter destructer
	GameEngineDepthStencil();
	~GameEngineDepthStencil();


	// delete Function
	GameEngineDepthStencil(const GameEngineDepthStencil& _Other) = delete;
	GameEngineDepthStencil(GameEngineDepthStencil&& _Other) noexcept = delete;
	GameEngineDepthStencil& operator=(const GameEngineDepthStencil& _Other) = delete;
	GameEngineDepthStencil& operator=(GameEngineDepthStencil&& _Other) noexcept = delete;

	void ReCreate(const D3D11_DEPTH_STENCIL_DESC& _Info);

	void Setting();

	void Reset();


protected:

private:
	D3D11_DEPTH_STENCIL_DESC Info_;
	ID3D11DepthStencilState* State_;

	void Create(const D3D11_DEPTH_STENCIL_DESC& _Info);
};

