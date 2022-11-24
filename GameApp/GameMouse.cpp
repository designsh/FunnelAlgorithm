#include "PreCompile.h"
#include "GameMouse.h"
#include "Enums.h"

#include <GameEngine/GameEngineCollision.h>

GameMouse::GameMouse() // default constructer ����Ʈ ������
	: mouseCollision_(nullptr), XYBuffer_(float4::ZERO)
{

}

GameMouse::~GameMouse() // default destructer ����Ʈ �Ҹ���
{

}

void GameMouse::ImageInit()
{
	UIRenderer = CreateTransformComponent<GameEngineUIRenderer>();
	UIRenderer->GetTransform()->SetLocalPosition({ 12.0f,-10.0f,0.0f });
	UIRenderer->GetTransform()->SetLocalScaling({ 30.0f, 30.0f,30.0f });
	SetCurSor("Cursor_01.png");
}

void GameMouse::CollisionInit()
{
	mouseCollision_ = CreateTransformComponent<GameEngineCollision>();
	mouseCollision_->GetTransform()->SetLocalScaling(float4{ 30.0f, 30.0f, 30.0f });
	mouseCollision_->SetCollisionInfo(CINT(CollisionGroup::MousePointer), CollisionType::AABBBox3D);
}

void GameMouse::Start()
{
	ImageInit();
	CollisionInit();
	
}

void GameMouse::Update(float _DeltaTime)
{
	InputCheck(_DeltaTime);
	UpdateMove(_DeltaTime);
	DebugUpdate(_DeltaTime);
}

void GameMouse::InputCheck(float _DeltaTime)
{
	// ���콺 �̵�


}


void GameMouse::UpdateMove(float _DeltaTime)
{
	Intersection(GameEngineInput::GetInst().GetMousePos().x, GameEngineInput::GetInst().GetMousePos().y);
	GetTransform()->SetWorldPosition(GameEngineInput::GetInst().GetMouse3DPos());
	XYBuffer_ = {GetTransform()->GetWorldPosition().x,GetTransform()->GetWorldPosition().z, GetTransform()->GetWorldPosition().y };
	mouseCollision_->GetTransform()->SetWorldPosition(XYBuffer_);
	
}

void GameMouse::DebugUpdate(float _DeltaTime)
{
	GetLevel()->PushDebugRender(mouseCollision_->GetTransform(), CollisionType::AABBBox3D);
}


void GameMouse::Intersection(float _mouseX, float _mouseY)
{
	// ���콺 Ŀ���� ��ǥ�� -1 ���� +1 �� ������ ����ȭ
	//float pointX = ((2.0f * (float)(_mouseX)) / (float)GameEngineWindow::GetInst().GetSize().x)  - 1.0f ;
	//float pointY = (((2.0f * (float)(_mouseY)) / (float)GameEngineWindow::GetInst().GetSize().y) - 1.0f) * -1.0f;
	//
	//// �� ��Ʈ�� ��Ⱦ�� ����ؼ� ���� ����� ����� ���� �����Ѵ�.
	//DirectX::XMMATRIX projection = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().Projection_.DirectMatrix;
	//DirectX::XMFLOAT4X4 projectionMatrix4;
	//
	//DirectX::XMStoreFloat4x4(&projectionMatrix4, projection);
	//
	//pointX = pointX / projectionMatrix4._11;
	//pointY = pointY / projectionMatrix4._22;
	//
	//// �� ����� ���Լ��� ���Ѵ�.
	//DirectX::XMMATRIX view = GetLevel()->GetMainCamera()->GetTransform()->GetTransformData().View_.DirectMatrix;
	//DirectX::XMMATRIX  inverseView = DirectX::XMMatrixInverse(nullptr, view);
	//
	//DirectX::XMFLOAT4X4 inverseViewMatrix4;
	//DirectX::XMStoreFloat4x4(&inverseViewMatrix4, inverseView);
	//
	//// �� �������� ��ŷ ������ ������ ����Ѵ�.
	//direction_.x = (pointX * inverseViewMatrix4._11) + (pointY * inverseViewMatrix4._21) + inverseViewMatrix4._31;
	//direction_.y = (pointX * inverseViewMatrix4._12) + (pointY * inverseViewMatrix4._22) + inverseViewMatrix4._32;
	//direction_.z = (pointX * inverseViewMatrix4._13) + (pointY * inverseViewMatrix4._23) + inverseViewMatrix4._33;
	//
	//// ī�޶��� ��ġ�� Picking Ray �� ������ �����´�.
	//pickingRayOrigin_ =	GetLevel()->GetMainCamera()->GetTransform()->GetWorldPosition();


}