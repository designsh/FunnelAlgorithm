#include "PreCompile.h"
#include "Yuki.h"

#include <GameEngine/GameEngineFBXMesh.h>
#include <GameEngine/GameEngineFBXRenderer.h>
#include <GameEngine/GameEngineCollision.h>

#include "FloorMap.h"
#include "NaviCell.h"
#include "Mouse.h"
#include "Ray.h"

Yuki* Yuki::MainPlayer = nullptr;

void Yuki::Initialize(NaviCell* _CurNaviCell, const float4& _InitPos)
{
	// ���� �÷��̾ ��ġ�� �ﰢ��(��)�� ����
	CurNaviCell_ = _CurNaviCell;

	// ���� �÷��̾ ��ġ�� �ﰢ��(��)�� �����߽����� ������ġ ����
	GetTransform()->SetWorldPosition(_InitPos);
}

void Yuki::MoveStart(NaviCell* _TargetNaviCell, const float4& _MoveTargetPos)
{
	// �̵�����
	MoveStart_ = false;

	// ���� �̵���ΰ� �����ִٸ� �̵��� Ÿ����ġ�� ����Ǿ����Ƿ�
	// ���� �÷��̾��� ��ġ��ǥ�� �ش��ϴ� �ﰢ���� �˾Ƴ���
	if (false == MovePath_.empty())
	{
		// ���� �̵���θ� �����ϰ�
		MovePath_.clear();

		// ���ִϸ��̼� ����
		AnimRenderer_->ChangeFBXAnimation(AnimNameList_[static_cast<int>(Yuki_State::COM_WAIT)]);

		// �÷��̾��� ������ġ��ǥ�� �̿��Ͽ� ���� ���� �ﰢ��(��)�� �˾Ƴ��� ����
		NaviCell* CurCell = FloorMap::ptrFloorMap->SearchCurrentPosToNaviCell(GetTransform()->GetWorldPosition());
		if (nullptr != CurCell)
		{
			CurNaviCell_ = CurCell;
		}
	}

	// �̵���ǥ������ �ﰢ�� ����
	TargetNaviCell_ = _TargetNaviCell;

	// ���� �÷��̾��� NaviCell ~ _TargetNaviCell������ �̵���� ����
	if (true == FloorMap::ptrFloorMap->MoveFacePath(GetTransform()->GetWorldPosition(), _MoveTargetPos, CurNaviCell_, TargetNaviCell_, MovePath_))
	{
		// �̵�������ġ ����
		MoveStartPos_ = MovePath_.front();
		MovePath_.pop_front();

		// ��λ��� �̵���ǥ��ġ ����
		MoveEndPos_ = MovePath_.front();
		MovePath_.pop_front();

		// �̵���� �����Ϸ� �� Flag On
		MoveStart_ = true;

		// �̵�����
		AnimRenderer_->ChangeFBXAnimation(AnimNameList_[static_cast<int>(Yuki_State::COM_RUN)]);
	}
}

void Yuki::Detect()
{
	// ������ Ÿ���� ����
	if (nullptr == CurTarget_)
	{
		return;
	}


}

void Yuki::Attack()
{
}

void Yuki::SkillAttack()
{
}

void Yuki::Block()
{
}

void Yuki::GetHit()
{
}

void Yuki::InputKeyStateCheck(float _DeltaTime)
{
	// �׽�ƮŰ üũ
	if (true == GameEngineInput::GetInst().Down("Yuki_AnimationIndexUP"))
	{
		++CurAnimationIndex_;
		if (CurAnimationIndex_ > static_cast<int>(AnimNameList_.size()) - 1)
		{
			CurAnimationIndex_ = 0;
		}
		AnimRenderer_->ChangeFBXAnimation(AnimNameList_[CurAnimationIndex_]);
	}

	if (true == GameEngineInput::GetInst().Down("Yuki_AnimationIndexDOWN"))
	{
		--CurAnimationIndex_;
		if (CurAnimationIndex_ < 0)
		{
			CurAnimationIndex_ = static_cast<int>(AnimNameList_.size()) - 1;
		}
		AnimRenderer_->ChangeFBXAnimation(AnimNameList_[CurAnimationIndex_]);
	}

	// �̵�üũ
	if (true == GameEngineInput::GetInst().Down("LBUTTON"))
	{
		//return;

		// ��, ȭ������� ����� ����
		if (true == GameEngineWindow::GetInst().IsWindowRangeOut(GameEngineInput::GetInst().GetMousePos()))
		{
			return;
		}

		if (nullptr != FloorMap::ptrFloorMap && nullptr != Mouse::MainMouse)
		{
			float4 MovePos = float4::ZERO;
			NaviCell* PickedNaviCell = Mouse::MainMouse->GetCurRay()->IsPickedCell(FloorMap::ptrFloorMap->GetFloorMapRenderer(), GameEngineInput::GetInst().GetMousePos(), MovePos);
			if (nullptr != PickedNaviCell)
			{
				MoveStart(PickedNaviCell, MovePos);
			}
		}
	}

	// Ÿ������üũ
	if (true == GameEngineInput::GetInst().Down("RBUTTON"))
	{
		//return;

		// ��, ȭ������� ����� ����
		if (true == GameEngineWindow::GetInst().IsWindowRangeOut(GameEngineInput::GetInst().GetMousePos()))
		{
			return;
		}

		// Ÿ������
		if (nullptr != Mouse::MainMouse)
		{
			float4 TargetPos = float4::ZERO;
			GameEngineActor* TargetActor = Mouse::MainMouse->GetCurRay()->IsPickedTarget(GameEngineInput::GetInst().GetMousePos(), TargetPos);
			if (nullptr != TargetActor)
			{
				// Ÿ�� ����
				CurTarget_ = TargetActor;

				// Ÿ�ٰ��� �Ÿ������Ͽ� �����Ÿ����� �̵�

				// Ÿ���� ���ݹ����� ���Խ� ���� ��� Ȱ��


				int a = 0;
			}
		}
	}
}

void Yuki::Start()
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
	MeshPath.MoveChild("Character");
	MeshPath.MoveChild("Yuki");

	GameEngineDirectory AnimationPath;
	AnimationPath.MoveParent("FunnelAlgorithm");
	AnimationPath.MoveChild("Resources");
	AnimationPath.MoveChild("FBX");
	AnimationPath.MoveChild("UserAni");
	AnimationPath.MoveChild("Character");
	AnimationPath.MoveChild("Yuki");
	AnimationPath.MoveChild("Common");

	//============================================== Create Animation Renderer ==============================================//
	AnimRenderer_ = CreateTransformComponent<GameEngineFBXRenderer>(GetTransform());
	AnimRenderer_->GetTransform()->SetLocalRotationDegree(float4(-90.0f, 0.0f, 0.0f, 0.0f));

	//====================================================== Mesh Load ======================================================//
	BaseMesh_ = GameEngineFBXMeshManager::GetInst().LoadUser(MeshPath.PathToPlusFileName("Yuki_BaseMesh.UserMesh"));
	AnimRenderer_->SetFBXMesh(BaseMesh_->GetName(), "TextureDeferredLightAni");
	AnimRenderer_->GetRenderSet(1).isRender = false;

	//==================================================== Animation Load ===================================================//
	// 00. Common
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_arrive.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_boxopen.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_collect.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_collect_Stone.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_collect_Water.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_collect_Wood.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_craftfood.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_craftmetal.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_dance.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_death.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_down_dead.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_down_run.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_down_start.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_down_wait.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_downdead.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_fishing.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_install_trap.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_operate.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_rest_end.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_rest_loop.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_rest_start.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_resurrect.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_run.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_skill04.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_skill04_end.UserAnimation"));
	GameEngineFBXAnimationManager::GetInst().LoadUser(AnimationPath.PathToPlusFileName("Yuki_Common_wait.UserAnimation"));

	// 01. 2Hand
	// 02. BareHands
	// 03. Dual
	// 04. Empty
	// 05. Lobby

	for (unsigned int i = 0; i < AnimRenderer_->GetRenderSetCount(); i++)
	{
		AnimRenderer_->GetRenderSet(i).ShaderHelper->SettingTexture("DiffuseTex", "Yuki_01_LOD1.png");
	}

	//=============================================== Create Animation Frame ================================================//
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_arrive", "Yuki_Common_arrive.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_boxopen", "Yuki_Common_boxopen.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_collect", "Yuki_Common_collect.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_collect_Stone", "Yuki_Common_collect_Stone.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_collect_Water", "Yuki_Common_collect_Water.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_collect_Wood", "Yuki_Common_collect_Wood.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_craftfood", "Yuki_Common_craftfood.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_craftmetal", "Yuki_Common_craftmetal.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_dance", "Yuki_Common_dance.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_death", "Yuki_Common_death.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_down_dead", "Yuki_Common_down_dead.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_down_run", "Yuki_Common_down_run.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_down_start", "Yuki_Common_down_start.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_down_wait", "Yuki_Common_down_wait.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_downdead", "Yuki_Common_downdead.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_fishing", "Yuki_Common_fishing.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_install_trap", "Yuki_Common_install_trap.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_operate", "Yuki_Common_operate.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_rest_end", "Yuki_Common_rest_end.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_rest_loop", "Yuki_Common_rest_loop.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_rest_start", "Yuki_Common_rest_start.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_resurrect", "Yuki_Common_resurrect.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_run", "Yuki_Common_run.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_skill04", "Yuki_Common_skill04.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_skill04_end", "Yuki_Common_skill04_end.UserAnimation", 0);
	AnimRenderer_->CreateFBXAnimation("Yuki_Common_wait", "Yuki_Common_wait.UserAnimation", 0);
	AnimRenderer_->ChangeFBXAnimation("Yuki_Common_wait");

	// �ִϸ��̼� �̸� ��� ����
	AnimNameList_ = AnimRenderer_->GetAnimationNameList();
#pragma endregion

#pragma region �浹ü ����
	//BodyCollider_
	//AttackCollider_

#pragma endregion

#pragma region �׽�ƮŰ ����
	if (false == GameEngineInput::GetInst().IsKey("Yuki_AnimationIndexUP"))
	{
		GameEngineInput::GetInst().CreateKey("Yuki_AnimationIndexUP", VK_UP);
	}

	if (false == GameEngineInput::GetInst().IsKey("Yuki_AnimationIndexDOWN"))
	{
		GameEngineInput::GetInst().CreateKey("Yuki_AnimationIndexDOWN", VK_DOWN);
	}

#pragma endregion
}

void Yuki::Update(float _DeltaTime)
{
#ifdef _DEBUG
	//GetLevel()->GetMainCamera()->PushDebugRender(GetTransform(), CollisionType::OBBBox3D);
#endif // _DEBUG

	// InputKey State Check
	InputKeyStateCheck(_DeltaTime);

	// �̵����� Flag On & �̵���ΰ� �����Ҷ� �÷��̾�� �̵�
	if (true == MoveStart_)
	{
		float4 CurPos = GetTransform()->GetWorldPosition();
		if ((MoveEndPos_ - CurPos).Len3D() > 1.f)
		{
			// �̵� �� ȸ��
			float4 MoveDir = (MoveEndPos_ - CurPos).NormalizeReturn3D();
			float MoveAngle = float4::DegreeDot3DToACosAngle(float4(0.0f, 0.0f, 1.0f, 0.0f), MoveDir);
			float4 MoveCross = float4::Cross3D(MoveDir, float4(0.0f, 0.0f, 1.0f, 0.0f)).NormalizeReturn3D();
			AnimRenderer_->GetTransform()->SetWorldRotationDegree(float4(-90.0f, MoveAngle * -MoveCross.y, 0.0f));
			GetTransform()->SetWorldPosition(CurPos + (MoveDir * MoveSpeed_ * _DeltaTime));
		}
		else // �̵������
		{
			// ���� ��ΰ� �����Ҷ�
			if (false == MovePath_.empty())
			{
				// ������ġ �缳��
				MoveStartPos_ = GetTransform()->GetWorldPosition();

				// ��ǥ��ġ �缳��
				MoveEndPos_ = MovePath_.front();
				MovePath_.pop_front();
			}
			else // ���̻��� ��ΰ� ��������������
			{
				// �̵����� �� �̵����� �ʱ�ȭ
				MoveStartPos_ = float4(0.0f, 0.0f, 0.0f, 0.0f);
				MoveEndPos_ = float4(0.0f, 0.0f, 0.0f, 0.0f);

				// ����θ� ����Ͽ� ������ ��ǥ��ġ�� �����ߴٸ�
				CurNaviCell_ = TargetNaviCell_;
				TargetNaviCell_ = nullptr;

				// �̵�����
				MoveStart_ = false;
				MovePath_.clear();

				// ������ �̵�����
				AnimRenderer_->ChangeFBXAnimation(AnimNameList_[static_cast<int>(Yuki_State::COM_WAIT)]);
			}
		}
	}
}

Yuki::Yuki()
	: BaseMesh_(nullptr)
	, AnimRenderer_(nullptr)
	, BodyCollider_(nullptr)
	, AttackCollider_(nullptr)
	, CurAnimationIndex_(0)
	, CurState_(Yuki_State::MAX)
	, CurNaviCell_(nullptr)
	, TargetNaviCell_(nullptr)
	, MoveStart_(false)
	, MoveStartPos_(float4(0.0f, 0.0f, 0.0f, 0.0f))
	, MoveEndPos_(float4(0.0f, 0.0f, 0.0f, 0.0f))
	, MoveSpeed_(10.0f)
	, CurTarget_(nullptr)
	, BattleMode_(false)
{
}

Yuki::~Yuki()
{
}
