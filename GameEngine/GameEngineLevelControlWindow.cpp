#include "PreCompile.h"
#include "GameEngineLevelControlWindow.h"
#include "GameEngineCore.h"
#include "GameEngineLevel.h"
#include <GameEngineBase\GameEngineTime.h>
#include "GameEngineActor.h"

GameEngineLevelControlWindow::GameEngineLevelControlWindow() 
{
}

GameEngineLevelControlWindow::~GameEngineLevelControlWindow() 
{
}

void GameEngineLevelControlWindow::AddText(const std::string& _str)
{
	texts_.push_back(_str);
}

void GameEngineLevelControlWindow::OnGUI()
{
	static float Acc = 1.0f;
	static std::string FrameText = "Frame : " + std::to_string(1.0f / GameEngineTime::GetInst().GetDeltaTime());

	Acc -= GameEngineTime::GetInst().GetDeltaTime();
	if (0 >= Acc)
	{
		FrameText = "Frame : " + std::to_string(1.0f / GameEngineTime::GetInst().GetDeltaTime());
		Acc = 1.0f;
	}
	ImGui::Text(FrameText.c_str());

	int Count = static_cast<int>(GameEngineCore::AllLevel_.size());

	for (auto& Level : GameEngineCore::AllLevel_)
	{
		if (true == ImGui::Button(Level.first.c_str()))
		{
			GameEngineCore::LevelChange(Level.first);
		}

		--Count;

		if (Count != 0)
		{
			ImGui::SameLine();
		}
	}

	for (const std::string& str : texts_)
	{
		ImGui::Text(str.c_str());
		ImGui::NextColumn();
	}

	texts_.clear();

	//GameEngineCore::CurrentLevel_->ActorList_;
	//std::vector<std::string> ActorName;
	//std::vector<const char*> ActorDisplay;
	//std::vector<GameEngineActor*> ActorObject;

	//for (auto& Group : GameEngineCore::CurrentLevel_->ActorList_)
	//{
	//	for (auto& Actor : Group.second)
	//	{
	//		if (Actor->GetName() == "")
	//		{
	//			ActorName.push_back("Actor");
	//			ActorDisplay.push_back("Actor");
	//		}
	//		else 
	//		{
	//			ActorName.push_back(Actor->GetName());
	//			ActorDisplay.push_back(ActorName[ActorName.size() -1].c_str());
	//		}

	//		ActorObject.push_back(Actor);
	//	}
	//}

	//static int SelectActor = -1;

	//ImGui::Text("ActorList");
	//ImGui::BeginListBox("", {500, 500});
	//ImGui::ListBox("", &SelectActor, &ActorDisplay[0], ActorDisplay.size());


	//if (-1 != SelectActor)
	//{
	//	int a = 0;
	//}

	//ImGui::EndListBox();

	// GameEngineCore::CurrentLevel_
}

