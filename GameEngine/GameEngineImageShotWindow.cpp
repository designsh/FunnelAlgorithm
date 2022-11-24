#include "PreCompile.h"
#include "GameEngineImageShotWindow.h"
#include <iostream>

GameEngineImageShotWindow::GameEngineImageShotWindow() 
{
}

GameEngineImageShotWindow::~GameEngineImageShotWindow() 
{
}

void GameEngineImageShotWindow::Start() 
{
	Style_ = ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize;
}

void GameEngineImageShotWindow::OnGUI()
{
	ImTextureID Id = reinterpret_cast<ImTextureID>(View_);
	// ImGui::Image(Id, { Size_.x, Size_.y });

	if (true == ImGui::ImageButton(Id, { Size_.x, Size_.y }))
	{
		Off();
	}
}