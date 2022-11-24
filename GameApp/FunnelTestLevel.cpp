#include "PreCompile.h"
#include "FunnelTestLevel.h"

#include <GameEngine/GameEngineGUI.h>
#include <GameEngine/GameEngineFBXWindow.h>
#include <GameEngine/GameEngineRenderWindow.h>
#include <GameEngine/SKySphereActor.h>
#include <GameEngine/LightActor.h>

#include "UserGame.h"
#include "FloorMap.h"
#include "Mouse.h"
#include "Yuki.h"
#include "Wolf.h"

#include "TestActor.h"

void FunnelTestLevel::LevelStart()
{
#pragma region IMGUI Windows
	//// ����Ÿ�� IMGUI Window
	//if (nullptr != GameEngineGUI::GetInst()->FindGUIWindow("RenderTargetWindow"))
	//{
	//	GameEngineRenderWindow* Window = GameEngineGUI::GetInst()->FindGUIWindowConvert<GameEngineRenderWindow>("RenderWindow");
	//	Window->PushRenderTarget("���� ī�޶� Ÿ��", GetMainCamera()->GetCameraRenderTarget(), float4(128.f, 72.f) * 3);
	//	Window->PushRenderTarget("UI ī�޶� Ÿ��", GetUICamera()->GetCameraRenderTarget(), float4(128.f, 72.f) * 3);
	//	Window->PushRenderTarget("���� ī�޶� ���۵�", GetMainCamera()->GetCameraDeferredGBufferTarget(), float4(128.f, 72.f) * 3);
	//	Window->PushRenderTarget("���� ī�޶� ���۵� ����Ʈ", GetMainCamera()->GetCameraDeferredLightTarget(), float4(128.f, 72.f) * 3);
	//	Window->PushRenderTarget("���� ī�޶� ���۵� ����Ʈ", GetMainCamera()->GetCameraDeferredTarget(), float4(128.f, 72.f) * 3);
	//}
#pragma endregion

#pragma region Ű���
	if (false == GameEngineInput::GetInst().IsKey("FreeCam"))
	{
		GameEngineInput::GetInst().CreateKey("FreeCam", 'o');
	}

	if (false == GameEngineInput::GetInst().IsKey("LBUTTON"))
	{
		GameEngineInput::GetInst().CreateKey("LBUTTON", VK_LBUTTON);
	}

	if (false == GameEngineInput::GetInst().IsKey("RBUTTON"))
	{
		GameEngineInput::GetInst().CreateKey("RBUTTON", VK_RBUTTON);
	}

#pragma endregion
}

void FunnelTestLevel::LevelUpdate(float _DeltaTime)
{
	// ���ҽ� �ε� �Ϸ�� ���ͻ���
	static bool ResourceLoadFlag = false;

	if (0 >= UserGame::LoadingFolder &&
		false == ResourceLoadFlag)
	{
		CreateActorLevel();
		ResourceLoadFlag = true;
	}

	// ����ī�޶���
	if (true == GameEngineInput::GetInst().Down("FreeCam"))
	{
		GetMainCameraActor()->FreeCameraModeSwitch();
	}

	if (true == GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}
	else
	{
		if (nullptr != Yuki_)
		{
			//float4 playerPosition = Yuki_->GetTransform()->GetWorldPosition();
			//GetMainCameraActor()->GetTransform()->SetWorldPosition(playerPosition + float4(400.f, 1280.f, -600.f));
			//GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 60.f, -35.f, 0.0f });
		}
	}

	// ...
}

void FunnelTestLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{

}

void FunnelTestLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{
	FBXWindow = GameEngineGUI::GetInst()->CreateGUIWindow<GameEngineFBXWindow>("FBXWindow");

	// FBXFolder : ���� �� FBX �� ��� �ִ� ����
	FBXWindow->FBXFolder.MoveParent("FunnelAlgorithm");
	FBXWindow->FBXFolder.MoveChild("Resources");
	FBXWindow->FBXFolder.MoveChild("FBX");
	FBXWindow->FBXFolder.MoveChild("SJH");

	// UserMeshSaveFolder : �޽� ���̳ʸ� ������ ���̺�� ����
	FBXWindow->UserMeshSaveFolder.MoveParent("FunnelAlgorithm");
	FBXWindow->UserMeshSaveFolder.MoveChild("Resources");
	FBXWindow->UserMeshSaveFolder.MoveChild("FBX");
	FBXWindow->UserMeshSaveFolder.MoveChild("UserMesh");
	FBXWindow->UserMeshSaveFolder.MoveChild("Character");
	FBXWindow->UserMeshSaveFolder.MoveChild("Yuki");

	// UserAnimationSaveFolder : �ִϸ��̼� ���̳ʸ� ������ ���̺�� ����
	FBXWindow->UserAnimationSaveFolder.MoveParent("FunnelAlgorithm");
	FBXWindow->UserAnimationSaveFolder.MoveChild("Resources");
	FBXWindow->UserAnimationSaveFolder.MoveChild("FBX");
	FBXWindow->UserAnimationSaveFolder.MoveChild("UserAni");
	FBXWindow->UserAnimationSaveFolder.MoveChild("Character");
	FBXWindow->UserAnimationSaveFolder.MoveChild("Yuki");
	FBXWindow->UserAnimationSaveFolder.MoveChild("Common");
}

void FunnelTestLevel::CreateActorLevel()
{
#pragma region ���ͻ���
	// ���
	Background_ = CreateActor<SKySphereActor>();

	// �÷��̾�(�����÷��̾�� ����)
	Yuki_ = CreateActor<Yuki>();
	Yuki::MainPlayer = Yuki_;

	// ����

	// 1. Wolf
	Wolf_ = CreateActor<Wolf>();
	Wolf::MainWolf = Wolf_;

	// 2. ...

	// ��
	FloorMap_ = CreateActor<FloorMap>();
	FloorMap::ptrFloorMap = FloorMap_;

	// ���콺
	InGameMouse_ = CreateActor<Mouse>();
	Mouse::MainMouse = InGameMouse_;

	// ����
	LightActor* Light1 = CreateActor<LightActor>();
	Light1->GetLight()->SetDiffusePower(1.f);
	Light1->GetLight()->SetAmbientPower(10.f);
	Light1->GetLight()->SetSpacularLightPow(10.0f);
#pragma endregion

	// ����ī�޶� ����
	GetMainCamera()->SetProjectionMode(ProjectionMode::Perspective);
	GetMainCameraActor()->GetCamera()->SetFov(50.f);
	GetMainCameraActor()->GetTransform()->SetWorldPosition({ 0.0f, 100.f, -200.f });
	//GetMainCameraActor()->FreeCameraModeSwitch();
}

FunnelTestLevel::FunnelTestLevel()
	: FBXWindow(nullptr)
	, Background_(nullptr)
	, FloorMap_(nullptr)
	, InGameMouse_(nullptr)
	, Yuki_(nullptr)
	, Wolf_(nullptr)
	, TestBox_(nullptr)
{

}

FunnelTestLevel::~FunnelTestLevel()
{
}
