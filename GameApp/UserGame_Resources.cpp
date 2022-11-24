#include "PreCompile.h"
#include "UserGame.h"
#include <GameEngine\EngineVertex.h>
#include "UserGame_Resources_Shader.h"
#include <GameEngine\GameEngineFontManager.h>

void TextureLoading(GameEngineDirectory Dir) 
{
	std::vector<GameEngineFile> AllFile = Dir.GetAllFile();

	for (size_t i = 0; i < AllFile.size(); i++)
	{
		GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	}

	--UserGame::LoadingFolder;
}

void UserGame::ResourcesLoad()
{
	GameEngineFontManager::GetInst().Load("궁서");


	{
		GameEngineDirectory SoundDir;
		SoundDir.MoveParent("FunnelAlgorithm");
		SoundDir.MoveChild("Resources");
		SoundDir.MoveChild("Sound");

		std::vector<GameEngineFile> AllFile = SoundDir.GetAllFile("mp3");

		for (size_t i = 0; i < AllFile.size(); i++)
		{
			GameEngineSoundManager::GetInst().Load(AllFile[i].GetFullPath());
		}

		// SoundDir / "aaaa" / "bbbb" / "cccc";
	}

	// 여기서는 글로벌만 로드해야 하고.
	{
		GameEngineDirectory TextureDir;
		TextureDir.MoveParent("FunnelAlgorithm");
		TextureDir.MoveChild("Resources");
		TextureDir.MoveChild("Image");

		std::vector<GameEngineDirectory> AllDir = TextureDir.GetAllDirectoryRecursive();
		LoadingFolder = static_cast<int>(AllDir.size());

		for (size_t i = 0; i < AllDir.size(); i++)
		{
			GameEngineCore::ThreadQueue.JobPost(std::bind(TextureLoading, AllDir[i]));
		}
		

		int a = 0;
		//std::vector<GameEngineFile> AllFile = TextureDir.GetAllFile();

		//for (size_t i = 0; i < AllFile.size(); i++)
		//{
		//	GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
		//}
	}

	// 이녀석들은 


	//{
	//	GameEngineDirectory TextureDir;
	//	TextureDir.MoveParent("AR38");
	//	TextureDir.MoveChild("Resources");
	//	TextureDir.MoveChild("Image");

	//	GameEngineFolderTextureManager::GetInst().Load(TextureDir.PathToPlusFileName("Dash"));
	//}



	// *** PJW 리소스 로딩 //
	{
		GameEngineDirectory TextureDir;
		TextureDir.MoveParent("FunnelAlgorithm");
		TextureDir.MoveChild("Resources");
		TextureDir.MoveChild("Texture");
		TextureDir.MoveChild("PJW");

		std::vector<GameEngineDirectory> AllDir = TextureDir.GetAllDirectoryRecursive();
		LoadingFolder = static_cast<int>(AllDir.size());

		for (size_t i = 0; i < AllDir.size(); i++)
		{
			GameEngineCore::ThreadQueue.JobPost(std::bind(TextureLoading, AllDir[i]));
		}
	}

	{
		GameEngineDirectory TextureDir;
		TextureDir.MoveParent("FunnelAlgorithm");
		TextureDir.MoveChild("Resources");
		TextureDir.MoveChild("Texture");
		TextureDir.MoveChild("YSJ");

		std::vector<GameEngineDirectory> AllDir = TextureDir.GetAllDirectoryRecursive();
		LoadingFolder = static_cast<int>(AllDir.size());

		for (size_t i = 0; i < AllDir.size(); i++)
		{
			GameEngineCore::ThreadQueue.JobPost(std::bind(TextureLoading, AllDir[i]));
		}
	}

	{
		GameEngineDirectory TextureDir;
		TextureDir.MoveParent("FunnelAlgorithm");
		TextureDir.MoveChild("Resources");
		TextureDir.MoveChild("Texture");
		TextureDir.MoveChild("SJH");

		std::vector<GameEngineDirectory> AllDir = TextureDir.GetAllDirectoryRecursive();
		LoadingFolder = static_cast<int>(AllDir.size());

		for (size_t i = 0; i < AllDir.size(); i++)
		{
			GameEngineCore::ThreadQueue.JobPost(std::bind(TextureLoading, AllDir[i]));
		}
	}

	AppShaderLoad();



}
