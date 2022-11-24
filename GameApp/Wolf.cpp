#include "Precompile.h"
#include "Wolf.h"

#include <GameEngine/GameEngineFBXMesh.h>
#include <GameEngine/GameEngineFBXRenderer.h>

#include "FloorMap.h"
#include "NaviCell.h"
#include "Mouse.h"
#include "Ray.h"

Wolf* Wolf::MainWolf = nullptr;

void Wolf::Initialize(NaviCell* _CurNaviCell, const float4& _InitPos)
{
	// 현재 플레이어가 위치한 삼각형(셀)을 지정
	CurNaviCell_ = _CurNaviCell;

	// 현재 플레이어가 위치한 삼각형(셀)의 무게중심으로 시작위치 셋팅
	GetTransform()->SetWorldPosition(_InitPos);
}

void Wolf::InputKeyStateCheck(float _DeltaTime)
{
	// 테스트키 체크
	if (true == GameEngineInput::GetInst().Down("Wolf_AnimationIndexUP"))
	{
		++CurAnimationIndex_;
		if (CurAnimationIndex_ > static_cast<int>(AnimNameList_.size()) - 1)
		{
			CurAnimationIndex_ = 0;
		}
		AnimRenderer_->ChangeFBXAnimation(AnimNameList_[CurAnimationIndex_]);
	}

	if (true == GameEngineInput::GetInst().Down("Wolf_AnimationIndexDOWN"))
	{
		--CurAnimationIndex_;
		if (CurAnimationIndex_ < 0)
		{
			CurAnimationIndex_ = static_cast<int>(AnimNameList_.size()) - 1;
		}
		AnimRenderer_->ChangeFBXAnimation(AnimNameList_[CurAnimationIndex_]);
	}
}

void Wolf::Start()
{
	//GetLevel()->GetMainCameraActor()->GetTransform()->AttachTransform(GetTransform());
	//GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalPosition({ 0.0f, 5.0f, 0.0f });
	//GetLevel()->GetMainCameraActor()->GetTransform()->SetLocalRotationDegree({ 80.0f, 0.0f, 0.0f });

#pragma region UserFiles Load
	// User FBX Files
	GameEngineDirectory MeshPath;
	MeshPath.MoveParent("FunnelAlgorithm");
	MeshPath.MoveChild("Resources");
	MeshPath.MoveChild("FBX");
	MeshPath.MoveChild("UserMesh");
	MeshPath.MoveChild("Monster");
	MeshPath.MoveChild("Wolf");

	GameEngineDirectory AnimationPath;
	AnimationPath.MoveParent("FunnelAlgorithm");
	AnimationPath.MoveChild("Resources");
	AnimationPath.MoveChild("FBX");
	AnimationPath.MoveChild("UserAni");
	AnimationPath.MoveChild("Monster");
	AnimationPath.MoveChild("Wolf");

	//============================================== Create Animation Renderer ==============================================//
	AnimRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	AnimRenderer_->GetTransform()->SetLocalRotationDegree(float4(-90.0f, 0.0f, 0.0f, 0.0f));

	//====================================================== Mesh Load ======================================================//
	BaseMesh_ = GameEngineFBXMeshManager::GetInst().LoadUser(MeshPath.PathToPlusFileName("Wolf_BaseMesh.UserMesh"));
	AnimRenderer_->SetFBXMesh(BaseMesh_->GetName(), "TextureDeferredLightAni");

	//==================================================== Animation Load ===================================================//
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_wake.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_wait.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_sleep_start.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_sleep.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_skill01.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_run.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_endbattle.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_death.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_dance.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_beware_start.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_beware_loop.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_beware_end.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_atk02.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_atk01.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_appearwait.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Wolf_01_appear.UserAnimation"));

	for (size_t i = 0; i < AnimRenderer_->GetRenderSetCount(); i++)
	{
		AnimRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Wolf_01.png");
	}

	//=============================================== Create Animation Frame ================================================//
	AnimRenderer_->CreateFBXAnimation("Wolf_01_wake", "Wolf_01_wake.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_wait", "Wolf_01_wait.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_sleep_start", "Wolf_01_sleep_start.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_sleep", "Wolf_01_sleep.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_skill01", "Wolf_01_skill01.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_run", "Wolf_01_run.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_endbattle", "Wolf_01_endbattle.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_death", "Wolf_01_death.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_dance", "Wolf_01_dance.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_beware_start", "Wolf_01_beware_start.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_beware_loop", "Wolf_01_beware_loop.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_beware_end", "Wolf_01_beware_end.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_atk02", "Wolf_01_atk02.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_atk01", "Wolf_01_atk01.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_appearwait", "Wolf_01_appearwait.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Wolf_01_appear", "Wolf_01_appear.UserAnimation", 0);
	AnimRenderer_->ChangeFBXAnimation("Wolf_01_wait");

	// 애니메이션 이름 목록 생성
	AnimNameList_ = AnimRenderer_->GetAnimationNameList();
#pragma endregion

#pragma region 충돌체 생성
	//BodyCollider_
	//AttackCollider_

#pragma endregion

#pragma region 테스트키 생성
	if (false == GameEngineInput::GetInst().IsKey("Wolf_AnimationIndexUP"))
	{
		GameEngineInput::GetInst().CreateKey("Wolf_AnimationIndexUP", VK_RIGHT);
	}

	if (false == GameEngineInput::GetInst().IsKey("Wolf_AnimationIndexDOWN"))
	{
		GameEngineInput::GetInst().CreateKey("Wolf_AnimationIndexDOWN", VK_LEFT);
	}
#pragma endregion
}

void Wolf::Update(float _DeltaTime)
{
	// InputKey State Check
	InputKeyStateCheck(_DeltaTime);


}

Wolf::Wolf()
	: BaseMesh_(nullptr)
	, AnimRenderer_(nullptr)
	, BodyCollider_(nullptr)
	, AttackColiider_(nullptr)
	, CurAnimationIndex_(0)
	, CurNaviCell_(nullptr)
	, TargetNaviCell_(nullptr)
	, MoveStart_(false)
{
}

Wolf::~Wolf()
{
}
