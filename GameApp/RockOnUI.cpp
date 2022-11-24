#include "PreCompile.h"
#include "RockOnUI.h"
#include "GameEngine/GameEngineUIRenderer.h"
#include "GameEngine/GameEngineInput.h"

#include "Player.h"

LockOnUI::LockOnUI()
	: Time(1.0f), UIOn(false)
{
}

LockOnUI::~LockOnUI()
{

}

void LockOnUI::Start()
{
	//�÷��̾ ������ Ÿ���� ������
	//GameEngineActor* Target = Player_->PlayerGetTarget();

	/*
	�׽�Ʈ �̹����� TestRockOn.png �� ����Ͻø� �˴ϴ�.
	
	RockOnUI�� �÷��̾� Ŭ���� �ȿ��� �����˴ϴ�.
	�÷��̾��� ��ü �Լ��� LockOnUI Ŭ������ L_SHift�� ������ On() �ǰ� �� ������ Off() �˴ϴ�.
	�÷��̾ Ÿ������ ��ǥ�� ��ǥ�� �����ؼ� UI�� �Ÿ�, ȸ�� ��� ���� ������ ũ��� Ÿ������ ��µǱ� �ٶ��ϴ�.
	
	
	
	*/



	{
		LockOnRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		LockOnRenderer->SetImage("TestRockOn.png", "PointSmp");
		LockOnRenderer->GetTransform()->SetLocalScaling(LockOnRenderer->GetCurrentTexture()->GetTextureSize());
		LockOnRenderer->GetTransform()->SetLocalPosition({0.0f,0.0f,0.0f,0.0f});
		LockOnRenderer->Off();
	}


	//���ҽ����� SetImage�ϰ� ��ġ�� �����մϴ�
	//��ġ�� ������ ���ҽ����� UIRenderMap�� String�� ���� insert�˴ϴ�.
	//�ΰ�������(HP�� ���¹̳� ����, ������񿩺�)���� ������, UIRendererMap���� �̹����̸����� Find�ؼ� ���ҽ��� �ٲ� �������ϴ�.
	//{
	//	//z���� �̿��� �տ����� �̹���/�ڿ����� �̹��� ������ ���ϰ� �ֽ��ϴ�.
	//	//��ġ������ �� float�� ���� �þ�� map�̳� vector�� ������ �����Դϴ�.
	//	float4 BackGroundBarPos = { -400.0f, -200.0f, 0.0f };
	//	float4 HPBarPos = { -395.0f, -202.0f, -1.0f };
	//	float4 HPBarTopPos = { -394.0f, -143.0f, -1.0f };
	//	float4 HPBarLinePos = { -401.0f, -180.0f, -2.0f};
	//	
	//	{
	//		TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	//		TopRenderer->SetImage("BackGroundBar_Test.png", "PointSmp");
	//		TopRenderer->GetTransform()->SetLocalPosition(BackGroundBarPos);
	//		TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
	//		UIRendererMap.insert(make_pair("BackGroundBar_Test", TopRenderer));
	//	}

	//	{
	//		TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	//		TopRenderer->SetImage("HPBar_example.png", "PointSmp");
	//		TopRenderer->GetTransform()->SetLocalPosition(HPBarPos);
	//		TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
	//		UIRendererMap.insert(make_pair("HPBar_example", TopRenderer));
	//	}

	//	{
	//		TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	//		TopRenderer->SetImage("HPBar_Top.png", "PointSmp");
	//		TopRenderer->GetTransform()->SetLocalPosition(HPBarTopPos);
	//		TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
	//		UIRendererMap.insert(make_pair("HPBar_Top", TopRenderer));
	//	}

	//	{
	//		TopRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
	//		TopRenderer->SetImage("Bar_Line.png", "PointSmp");
	//		TopRenderer->GetTransform()->SetLocalPosition(HPBarLinePos);
	//		TopRenderer->GetTransform()->SetLocalScaling(TopRenderer->GetCurrentTexture()->GetTextureSize());
	//		UIRendererMap.insert(make_pair("Bar_Line", TopRenderer));
	//	}
	//}


	//����, Player ���� �Լ�
	//int HP = Player_->PlayerGetHP();
	//float Stmina = Player_->PlayerGetStamina();
	//Player_->PlayerSetHP(10);
	//Player_->PlayerSetStamina(100.f);
}

void LockOnUI::Update(float _Time)
{


	GameEngineActor* Target = Player_->PlayerGetTarget();

	//Ÿ���� ������ ��ġ������ �������� LockOnRenderer�� ON
	if (Target != nullptr)
	{
	float4 position = Target->GetTransform()->GetWorldPosition();
	LockOnRenderer->On();
	LockOnRenderer->GetTransform()->SetLocalPosition(GetLockOnPosition(position));
//	//GetLockOnPosition(position);
	//3D��ġ�� ���� 2D��ġ�� �޾ƿ;���
	//LockOnRenderer->GetTransform()->SetLocalPosition();
	}
	else
	{
		//Ÿ���� null�̸� LockOnRenderer�� ����
		LockOnRenderer->Off();
	}


}

