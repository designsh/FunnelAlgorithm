#pragma once
#include "GameEnginePostProcessRender.h"

// Ό³Έν :
class DeferredMerge : public GameEnginePostProcessRender
{
public:
	// constrcuter destructer
	DeferredMerge();
	~DeferredMerge();

	// delete Function
	DeferredMerge(const DeferredMerge& _Other) = delete;
	DeferredMerge(DeferredMerge&& _Other) noexcept = delete;
	DeferredMerge& operator=(const DeferredMerge& _Other) = delete;
	DeferredMerge& operator=(DeferredMerge&& _Other) noexcept = delete;

	void Effect(float _DeltaTime) override;
	void Initialize() override;

	void SetDeferredTarget(GameEngineRenderTarget* _GBuffer, GameEngineRenderTarget* _Light);

protected:

private:

};

