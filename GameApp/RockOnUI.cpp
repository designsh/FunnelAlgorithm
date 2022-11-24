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
	//플레이어가 락온한 타겟의 포인터
	//GameEngineActor* Target = Player_->PlayerGetTarget();

	/*
	테스트 이미지로 TestRockOn.png 를 사용하시면 됩니다.
	
	RockOnUI는 플레이어 클레스 안에서 생성됩니다.
	플레이어의 자체 함수로 LockOnUI 클레스는 L_SHift를 누르면 On() 되고 안 누르면 Off() 됩니다.
	플레이어가 타겟팅한 목표의 좌표를 참고해서 UI가 거리, 회전 상관 없이 일정한 크기로 타겟위에 출력되길 바랍니다.
	
	
	
	*/



	{
		LockOnRenderer = CreateTransformComponent<GameEngineUIRenderer>(GetTransform());
		LockOnRenderer->SetImage("TestRockOn.png", "PointSmp");
		LockOnRenderer->GetTransform()->SetLocalScaling(LockOnRenderer->GetCurrentTexture()->GetTextureSize());
		LockOnRenderer->GetTransform()->SetLocalPosition({0.0f,0.0f,0.0f,0.0f});
		LockOnRenderer->Off();
	}


	//리소스들을 SetImage하고 위치를 조정합니다
	//위치를 조정한 리소스들은 UIRenderMap에 String과 같이 insert됩니다.
	//인게임정보(HP나 스태미너 상태, 착용장비여부)등을 받으면, UIRendererMap에서 이미지이름으로 Find해서 리소스를 바꿀 예정업니다.
	//{
	//	//z값을 이용해 앞에오는 이미지/뒤에오는 이미지 순서를 정하고 있습니다.
	//	//위치정보가 될 float도 양이 늘어나면 map이나 vector로 관리할 예정입니다.
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


	//이현, Player 예제 함수
	//int HP = Player_->PlayerGetHP();
	//float Stmina = Player_->PlayerGetStamina();
	//Player_->PlayerSetHP(10);
	//Player_->PlayerSetStamina(100.f);
}

void LockOnUI::Update(float _Time)
{


	GameEngineActor* Target = Player_->PlayerGetTarget();

	//타겟이 있으면 위치정보를 가져오고 LockOnRenderer가 ON
	if (Target != nullptr)
	{
	float4 position = Target->GetTransform()->GetWorldPosition();
	LockOnRenderer->On();
	LockOnRenderer->GetTransform()->SetLocalPosition(GetLockOnPosition(position));
//	//GetLockOnPosition(position);
	//3D위치를 토대로 2D위치를 받아와야함
	//LockOnRenderer->GetTransform()->SetLocalPosition();
	}
	else
	{
		//타겟이 null이면 LockOnRenderer를 끈다
		LockOnRenderer->Off();
	}


}

