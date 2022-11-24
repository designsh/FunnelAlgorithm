#pragma once
#include <GameEngine/GameEngineActor.h>

using namespace std;
// 설명 :
class GameEngineUIRenderer;
class Player;
class LockOnUI : public GameEngineActor
{
public:
	// constrcuter destructer
	LockOnUI();
	~LockOnUI();

	// delete Function
	LockOnUI(const LockOnUI& _Other) = delete;
	LockOnUI(LockOnUI&& _Other) noexcept = delete;
	LockOnUI& operator=(const LockOnUI& _Other) = delete;
	LockOnUI& operator=(LockOnUI&& _Other) noexcept = delete;

	bool UIOn;

protected:
	GameEngineUIRenderer* LockOnRenderer;
	map<string, GameEngineUIRenderer*> UIRendererMap;
	float Time;


private:
	void Start() override;
	void Update(float _Time) override;

private:
	D3D11_VIEWPORT GameViewPort;

private:
	Player* Player_;

public:
	void SetPlayer(Player* _Player)
	{
		Player_ = _Player;
	}
	
private:
	float4 GetLockOnPosition(float4 _Position)
	{
		//3D공간의 좌표를 2D화면좌표로 변환

		DirectX::XMVECTOR Position = _Position.DirectVector;
		D3D11_VIEWPORT ViewPort_;

		UINT Number = 1;

		GameEngineDevice::GetContext()->RSGetViewports(&Number, &ViewPort_);

		float4x4 Pro = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_;
		float4x4 View = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_;
		DirectX::XMMATRIX ViewProjectionMatrix = View.DirectMatrix *= Pro.DirectMatrix;
		Position = DirectX::XMVector3TransformCoord(Position, ViewProjectionMatrix);
		float4 ReturnPosition = {0.0f, 0.0f};
		ReturnPosition.DirectVector = Position;

		ReturnPosition.DxXmfloat3 = DirectX::XMFLOAT3(ViewPort_.Width * (ReturnPosition.x + 1.0f) / 2.0f + ViewPort_.TopLeftX, ViewPort_.Height * (2.0f - (ReturnPosition.y + 1.0f)) / 2.0f + ViewPort_.TopLeftY, 0.0f);

		ReturnPosition.x -= (ViewPort_.Width / 2);
		ReturnPosition.y = (ReturnPosition.y - (ViewPort_.Height / 2)) * -1.0f;
		return ReturnPosition;
	}
};

