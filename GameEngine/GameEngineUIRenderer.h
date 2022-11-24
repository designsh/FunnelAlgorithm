#pragma once
#include "GameEngineImageRenderer.h"

// ���� : �ϳ��� ���� ������ ǥ���մϴ�.
class CameraComponent;
class GameEngineLevel;
class GameEngineRenderingPipeLine;
class GameEngineUIRenderer : public GameEngineImageRenderer
{
private:
	friend GameEngineLevel;
	friend CameraComponent;

public:
	// constrcuter destructer
	GameEngineUIRenderer();
	~GameEngineUIRenderer();

	// delete Function
	GameEngineUIRenderer(const GameEngineUIRenderer& _Other) = delete;
	GameEngineUIRenderer(GameEngineUIRenderer&& _Other) noexcept = delete;
	GameEngineUIRenderer& operator=(const GameEngineUIRenderer& _Other) = delete;
	GameEngineUIRenderer& operator=(GameEngineUIRenderer&& _Other) noexcept = delete;

	void SetRenderGroup(int _Order) override;

	void TextSetting(std::string _FontName, std::string _PrintText, float _FontSize, float4 _Color = float4::RED);

protected:
	std::string FontName_;
	std::string PrintText_;
	float FontSize_;
	float4 FontPivot_;
	float4 Color_;

	// ���ļ� ����Ϸ��� ���� �ʰ���?
	static int UIRendererCount;
	static GameEngineRenderTarget* FontTarget_;




private:
	void Start() override;

	void Render(float _DeltaTime, bool _IsDeferred) override;
};

