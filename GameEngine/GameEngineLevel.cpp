#include "PreCompile.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include "GameEngineRenderer.h"
#include "GameEngineDevice.h"
#include "GameEngineTransform.h"
#include "CameraActor.h"
#include "CameraComponent.h"
#include "GameEngineCollision.h"
#include "GameEngineDebugRenderData.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineFontManager.h"
#include "GameEngineFont.h"
#include "GameEngineUIRenderer.h"
#include "GameEngineGUI.h"
#include <GameEngine\GameEnginePostProcessRender.h>


CameraActor* GameEngineLevel::GetMainCameraActor()
{
	return MainCameraActor_;
}

CameraComponent* GameEngineLevel::GetMainCamera()
{
	return MainCameraActor_->GetCamera();
}

CameraActor* GameEngineLevel::GetUICameraActor()
{
	return UICameraActor_;
}

CameraComponent* GameEngineLevel::GetUICamera()
{
	return UICameraActor_->GetCamera();
}
GameEngineLevel::GameEngineLevel()
	: MainCameraActor_(nullptr)
	, UICameraActor_(nullptr)
{
	PostRender["CameraMergePrev"];
	PostRender["CameraMergeNext"];
}

GameEngineLevel::~GameEngineLevel()
{
	ClearAll();
}

void GameEngineLevel::Init()
{
	MainCameraActor_ = CreateActor<CameraActor>();
	UICameraActor_ = CreateActor<CameraActor>();

	UICameraActor_->GetCamera()->SetProjectionMode(ProjectionMode::Orthographic);
	UICameraActor_->GetTransform()->SetLocalPosition(float4(0.0f, 0.0f, -100.0f));
}

void GameEngineLevel::ActorUpdate(float _DeltaTime)
{
	TimeEventUpdate();

	for (std::pair<int, std::list<GameEngineActor*>> Pair : ActorList_)
	{
		std::list<GameEngineActor*>& Actors = Pair.second;

		float Scale = GameEngineTime::GetInst().GetTimeScale(Pair.first);

		for (GameEngineActor* Actor : Actors)
		{
			if (false == Actor->IsUpdate())
			{
				continue;
			}

			// 위치바꾸고
			Actor->Update(_DeltaTime * Scale);
			Actor->UpdateComponent(_DeltaTime * Scale);
		}
	}
}

void GameEngineLevel::LevelChangeEndActorEvent(GameEngineLevel* _NextLevel)
{
	for (std::pair<int, std::list<GameEngineActor*>> Pair : ActorList_)
	{
		std::list<GameEngineActor*>& Actors = Pair.second;

		// Actors End
		for (GameEngineActor* Actor : Actors)
		{
			Actor->LevelChangeEndEvent(_NextLevel);
		}
	}
}
void GameEngineLevel::LevelChangeStartActorEvent(GameEngineLevel* _PrevLevel)
{
	for (std::pair<int, std::list<GameEngineActor*>> Pair : ActorList_)
	{
		std::list<GameEngineActor*>& Actors = Pair.second;

		for (GameEngineActor* Actor : Actors)
		{
			Actor->LevelChangeStartEvent(_PrevLevel);
		}
	}
}

void GameEngineLevel::Render(float _DeltaTime)
{
	GameEngineDevice::RenderStart();

	MainCameraActor_->GetCamera()->ClearCameraTarget();
	UICameraActor_->GetCamera()->ClearCameraTarget();
	MainCameraActor_->GetCamera()->Render(_DeltaTime);
	MainCameraActor_->GetCamera()->DebugRender();
	
	UICameraActor_->GetCamera()->Render(_DeltaTime);
	UICameraActor_->GetCamera()->DebugRender();

	{
		std::vector<GameEnginePostProcessRender*>& PostCameraMergePrev = PostRender["CameraMergePrev"];
		for (size_t i = 0; i < PostCameraMergePrev.size(); i++)
		{
			PostCameraMergePrev[i]->Effect(_DeltaTime);
		}
	}

	GameEngineDevice::GetBackBufferTarget()->Merge(MainCameraActor_->GetCamera()->GetCameraRenderTarget());
	GameEngineDevice::GetBackBufferTarget()->Merge(UICameraActor_->GetCamera()->GetCameraRenderTarget());

	{
		std::vector<GameEnginePostProcessRender*>& PostCameraMergeNext = PostRender["CameraMergeNext"];
		for (size_t i = 0; i < PostCameraMergeNext.size(); i++)
		{
			PostCameraMergeNext[i]->Effect(_DeltaTime);
		}
	}


	GameEngineGUI::GetInst()->GUIRenderStart();
	GameEngineGUI::GetInst()->GUIRenderEnd();

	// 충돌체 랜더링이 무조건 화면에 뚫고 나와야하는 애들은
	GameEngineDevice::RenderEnd();


}

void GameEngineLevel::Release(float _DeltaTime)
{
	for (std::pair<int, std::list<GameEngineActor*>> Pair : ActorList_)
	{
		std::list<GameEngineActor*>& Actors = Pair.second;

		for (GameEngineActor* Actor : Actors)
		{
			Actor->ReleaseUpdate(_DeltaTime);
		}
	}

	MainCameraActor_->GetCamera()->ReleaseRenderer();
	UICameraActor_->GetCamera()->ReleaseRenderer();


	// 콜리전 삭제
	{
		std::map<int, std::list<GameEngineCollision*>>::iterator RenderMapBeginIter = CollisionList_.begin();
		std::map<int, std::list<GameEngineCollision*>>::iterator RenderMapEndIter = CollisionList_.end();


		for (; RenderMapBeginIter != RenderMapEndIter; ++RenderMapBeginIter)
		{
			std::list<GameEngineCollision*>& Collisions = RenderMapBeginIter->second;

			std::list<GameEngineCollision*>::iterator BeginIter = Collisions.begin();
			std::list<GameEngineCollision*>::iterator EndIter = Collisions.end();

			for (; BeginIter != EndIter; )
			{
				GameEngineCollision* ReleaseCollision = *BeginIter;

				if (nullptr == ReleaseCollision)
				{
					GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
				}

				if (true == ReleaseCollision->IsDeath())
				{
					BeginIter = Collisions.erase(BeginIter);

					continue;
				}

				++BeginIter;

			}
		}
	}

	{
		std::map<int, std::list<GameEngineActor*>>::iterator ActorMapBeginIter = ActorList_.begin();
		std::map<int, std::list<GameEngineActor*>>::iterator ActorMapEndIter = ActorList_.end();

		for (; ActorMapBeginIter != ActorMapEndIter; ++ActorMapBeginIter)
		{
			std::list<GameEngineActor*>& Actors = ActorMapBeginIter->second;

			std::list<GameEngineActor*>::iterator BeginIter = Actors.begin();
			std::list<GameEngineActor*>::iterator EndIter = Actors.end();

			for (; BeginIter != EndIter; )
			{
				GameEngineActor* ReleaseActor = *BeginIter;

				if (nullptr == ReleaseActor)
				{
					GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
					return;
				}

				ReleaseActor->ComponentRelease();


				if (true == ReleaseActor->IsDeath())
				{
					delete* BeginIter;
					*BeginIter = nullptr;

					BeginIter = Actors.erase(BeginIter);

					continue;
				}

				++BeginIter;
			}

		}
	}

}

// 	RendererList_[_Order].push_back(_Renderer);


void GameEngineLevel::LevelChangeStartEvent(GameEngineLevel* _PrevLevel)
{

}

void GameEngineLevel::LevelChangeEndEvent(GameEngineLevel* _NextLevel)
{

}

void GameEngineLevel::PushCollision(GameEngineCollision* _Collision, int _Group)
{
	CollisionList_[_Group].push_back(_Collision);
}

void GameEngineLevel::ChangeCollisionGroup(int _Group, GameEngineCollision* _Collision)
{
	CollisionList_[_Collision->GetOrder()].remove(_Collision);

	_Collision->SetOrder(_Group);

	CollisionList_[_Collision->GetOrder()].push_back(_Collision);
}

void GameEngineLevel::PushDebugRender(GameEngineTransform* _Transform, CollisionType _Type, float4 _Color)
{
	MainCameraActor_->GetCamera()->PushDebugRender(_Transform, _Type, _Color);
}

void GameEngineLevel::PushDebugRenderUI(GameEngineTransform* _Transform, CollisionType _Type, float4 _Color)
{
	UICameraActor_->GetCamera()->PushDebugRender(_Transform, _Type, _Color);
}

void GameEngineLevel::AddTimeEvent(float _Time, std::function<void()> _Event) 
{
	AddEvent_.push_back(new TimeEvent{ _Time, _Event });
}

void GameEngineLevel::TimeEventUpdate()
{
	for (auto& Event : AddEvent_)
	{
		AllEvent_.push_back(Event);
	}
	AddEvent_.clear();

	for (auto& Event : AllEvent_)
	{
		Event->Time_ -= GameEngineTime::GetInst().GetDeltaTime();
		if (0 >= Event->Time_)
		{
			Event->Event_();
		}
	}

	std::list<TimeEvent*>::iterator StartIter = AllEvent_.begin();
	std::list<TimeEvent*>::iterator EndIter = AllEvent_.end();

	for (; StartIter != EndIter; )
	{
		if (0 >= (*StartIter)->Time_)
		{
			delete* StartIter;
			StartIter = AllEvent_.erase(StartIter);
			continue;
		}
		++StartIter;
	}
}

void GameEngineLevel::ClearAll()
{
	for (auto& Event : AllEvent_)
	{
		delete Event;
	}

	AddEvent_.clear();

	for (auto& Effects : PostRender)
	{
		for (auto& Effect : Effects.second)
		{
			delete Effect;
		}
	}


	for (std::pair<int, std::list<GameEngineActor*>> Pair : ActorList_)
	{
		std::list<GameEngineActor*>& Actors = Pair.second;

		for (GameEngineActor* Actor : Actors)
		{
			if (nullptr != Actor)
			{
				delete Actor;
				Actor = nullptr;
			}

		}
	}
}

void GameEngineLevel::SetLevelActorMoveProcess()
{
	for (size_t i = 0; i < NextLevelActorsData_.size(); i++)
	{
		GameEngineLevel* _NextLevel = NextLevelActorsData_[i].Level;
		GameEngineActor* _Actor = NextLevelActorsData_[i].Actor;

		// 지금 내 카메라에서 어떠한 랜더러를 빼서
		MainCameraActor_->GetCamera()->NextLevelMoveRenderer(_NextLevel->GetMainCamera(), _Actor);
		UICameraActor_->GetCamera()->NextLevelMoveRenderer(_NextLevel->GetUICamera(), _Actor);

		// 콜리전 삭제
		{
			std::map<int, std::list<GameEngineCollision*>>::iterator RenderMapBeginIter = CollisionList_.begin();
			std::map<int, std::list<GameEngineCollision*>>::iterator RenderMapEndIter = CollisionList_.end();


			for (; RenderMapBeginIter != RenderMapEndIter; ++RenderMapBeginIter)
			{
				std::list<GameEngineCollision*>& Collisions = RenderMapBeginIter->second;

				std::list<GameEngineCollision*>::iterator BeginIter = Collisions.begin();
				std::list<GameEngineCollision*>::iterator EndIter = Collisions.end();

				for (; BeginIter != EndIter; )
				{
					GameEngineCollision* ReleaseCollision = *BeginIter;

					if (nullptr == ReleaseCollision)
					{
						GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
					}

					if (ReleaseCollision->GetActor() == _Actor)
					{
						_NextLevel->CollisionList_[(*BeginIter)->GetOrder()].push_back(*BeginIter);

						BeginIter = Collisions.erase(BeginIter);

						continue;
					}

					++BeginIter;

				}
			}
		}

		{
			std::map<int, std::list<GameEngineActor*>>::iterator ActorMapBeginIter = ActorList_.begin();
			std::map<int, std::list<GameEngineActor*>>::iterator ActorMapEndIter = ActorList_.end();

			for (; ActorMapBeginIter != ActorMapEndIter; ++ActorMapBeginIter)
			{
				std::list<GameEngineActor*>& Actors = ActorMapBeginIter->second;

				std::list<GameEngineActor*>::iterator BeginIter = Actors.begin();
				std::list<GameEngineActor*>::iterator EndIter = Actors.end();

				for (; BeginIter != EndIter; )
				{
					GameEngineActor* ReleaseActor = *BeginIter;

					if (nullptr == ReleaseActor)
					{
						GameEngineDebug::MsgBoxError("Release Actor Is Nullptr!!!!");
					}

					if (ReleaseActor == _Actor)
					{
						_NextLevel->ActorList_[ReleaseActor->GetOrder()].push_back(ReleaseActor);

						ReleaseActor->SetLevel(_NextLevel);

						BeginIter = Actors.erase(BeginIter);

						continue;
					}

					++BeginIter;
				}

			}
		}
	}

	NextLevelActorsData_.clear();
}

void GameEngineLevel::SetLevelActorMove(GameEngineLevel* _NextLevel, GameEngineActor* _Actor)
{
	NextLevelActorsData_.push_back({ _Actor , _NextLevel });


}