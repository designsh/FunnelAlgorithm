#include "PreCompile.h"
#include "GameEngineFolderTexture.h"
#include "GameEngineTexture.h"
#include "GameEngineBase/GameEngineFile.h"
#include "GameEngineBase/GameEngineString.h"
#include "GameEngineBase/GameEngineDirectory.h"

#pragma comment(lib, "DirectXTex.lib")

GameEngineFolderTexture::GameEngineFolderTexture() // default constructer 디폴트 생성자
{

}

GameEngineFolderTexture::~GameEngineFolderTexture() // default destructer 디폴트 소멸자
{
	for (size_t i = 0; i < AllTexture_.size(); i++)
	{
		if (nullptr != AllTexture_[i])
		{
			delete AllTexture_[i];
			AllTexture_[i] = nullptr;
		}
	}
}


void GameEngineFolderTexture::Load(const std::string& _Path)
{
	GameEngineDirectory TextureDir = _Path;

	std::vector<GameEngineFile> AllFile = TextureDir.GetAllFile();

	for (size_t i = 0; i < AllFile.size(); i++)
	{
		GameEngineTexture* NewRes = new GameEngineTexture();
		NewRes->SetName(AllFile[i].FileName());
		NewRes->Load(AllFile[i].GetFullPath());
		AllTexture_.push_back(NewRes);
		//GameEngineTextureManager::GetInst().Load(AllFile[i].GetFullPath());
	}

}

GameEngineTexture* GameEngineFolderTexture::GetTextureIndex(int _Index) 
{
	return AllTexture_[_Index];
}