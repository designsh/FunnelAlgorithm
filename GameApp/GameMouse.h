#pragma once
#include <GameEngine/MouseActor.h>



class PlayerController;
class Player;
class GameEngineCollision;
class GameMouse : public MouseActor
{
	friend PlayerController;
	friend Player;

public:
	GameMouse(); // default constructer ����Ʈ ������
	~GameMouse(); // default destructer ����Ʈ �Ҹ���
	GameMouse(const GameMouse& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameMouse(GameMouse&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������
	GameMouse& operator=(const GameMouse& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameMouse& operator=(const GameMouse&& _other) noexcept = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

protected:
	// Start() �� �� �ʱ�ȭ �Լ���
	void ImageInit();
	void CollisionInit();

	// Update() �� ���� �ֱ������� üũ�ϴ� �Լ���
	void InputCheck(float _DeltaTime); // ���콺 �� �� Ŭ��, ��ũ�Ѹ��� üũ�մϴ�.
	void DebugUpdate(float _DeltaTime);


	GameEngineCollision* mouseCollision_;
	float4 XYBuffer_;
	void UpdateMove(float _DeltaTime);
	// ���콺 �浹ü�� �� ���� �� �ٴ��� Ÿ�� �ٴ� �� �ְ� �ϴ� �Լ�(���콺�� XY ���� �ΰ����� XZ ������ ��ȯ)

private:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	float4 savedPos_;
	// ���콺 �̵� �� ���콺�� ��ǥ�� �����ϴ� ����





	void SaveCurPos()
	{
		savedPos_ = GameEngineInput::GetInst().GetMouse3DPos();
			//UIRenderer->GetTransform()->GetWorldPosition();
	}
	
public:
	const float4 GetSavedPos()
	{
		return savedPos_;
	}



private:
	// ��ŷ ������ �Լ���

	float4 pickingRayDirection_; // ������ ����
	float4 pickingRayOrigin_; // ������ ����
	bool Init(HINSTANCE _hinstance, HWND _hwnd, int _a, int _b);

	void Intersection(float _mouseX, float _mouseY);


	class Ray
	{
	public:
		Ray();
		~Ray();

	protected:
		float4 original_; // ������ ��� ����
		float4 direction_; // ������ ����(ȭ�� �������� ���ϰ�)

	public:
		Ray RayAtViewSpace(int nScreenX, int nScreenY);
		// ����̽����� ����Ʈ�� ������� ���� ����
		// ��꿡 �ʿ��� ���⺤�� direction_ �� ���
		// Z ����? 1.0f?
		

		Ray RayAtWorldSpace(int nScreenX, int nScreenY);
		// ����Ʈ, ��������� �̿��� ��ȯ�� ���⺤�͸� ���´�.
		// ����� ��� ���� ���� 2���� ���� (matView, matInvView)
		// d3dDevice �� �̿��� matView �� ������ ������� ��´�.
		// matInvView��Ŀ� matView�� ������� ��´�.
		// ���� ������� �̿��ؼ� ������ ������� ��ȯ�Ѵ�.
		// ... �̰ź��Ͱ� ���� ������.
		// 
		// 
		// ���� ������� �̿��ؼ� ���⺤�͸� ��ȯ�Ѵ�.
		// ���⺤�͸� ����ȭ(��ľ��� ��簪�� 1���� �۵���)�Ѵ�.

		// �� �� �����̽��� ����Ǳ� ���� ��ǥ�� �ʿ��ϴ�...


	};
public:
	


};

