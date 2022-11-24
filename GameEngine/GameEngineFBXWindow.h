#pragma once
#include "GameEngineGUI.h"
#include "GameEngineRenderTarget.h"
#include <unordered_map>
#include "GameEngineFBXMeshManager.h"
#include <GameEngineBase/GameEngineDirectory.h>
#include <GameEngineBase/GameEngineFile.h>

// Ό³Έν :
class GameEngineFBXWindow : public GameEngineGUIWindow
{
	struct RenderTargetDrawData
	{
	public:
		std::string Name;
		GameEngineRenderTarget* Target;
		float4 Size_;
		bool ScaleCheck;
	};

public:
	// constrcuter destructer
	GameEngineFBXWindow();
	~GameEngineFBXWindow();

	// delete Function
	GameEngineFBXWindow(const GameEngineFBXWindow& _Other) = delete;
	GameEngineFBXWindow(GameEngineFBXWindow&& _Other) noexcept = delete;
	GameEngineFBXWindow& operator=(const GameEngineFBXWindow& _Other) = delete;
	GameEngineFBXWindow& operator=(GameEngineFBXWindow&& _Other) noexcept = delete;

	void TestInit();

protected:

	void OnGUI() override;

public:
	GameEngineDirectory FBXFolder;


	GameEngineDirectory UserMeshSaveFolder;
	GameEngineDirectory UserAnimationSaveFolder;

	int FBXFileSelect;
	int ActorSelect;

	std::vector<std::string> ActorsNames;
	std::vector<std::string> OriNames;
	std::vector<std::string> Names;

	std::vector<GameEngineActor*> Actors;

	class GameEngineFBXAnimation* SelectAnimation;

private:
	// GameEngineFBXMesh FBXMesh;

	void ActorControl();

};

