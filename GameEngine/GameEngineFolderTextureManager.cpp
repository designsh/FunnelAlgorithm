#include "PreCompile.h"
#include "GameEngineFolderTextureManager.h"
#include "GameEngineFolderTexture.h"

GameEngineFolderTextureManager* GameEngineFolderTextureManager::Inst = new GameEngineFolderTextureManager();

GameEngineFolderTextureManager::GameEngineFolderTextureManager() // default constructer 디폴트 생성자
{

}

GameEngineFolderTextureManager::~GameEngineFolderTextureManager() // default destructer 디폴트 소멸자
{
	for (const std::pair<std::string, GameEngineFolderTexture*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineFolderTextureManager::GameEngineFolderTextureManager(GameEngineFolderTextureManager&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

GameEngineFolderTexture* GameEngineFolderTextureManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

GameEngineFolderTexture* GameEngineFolderTextureManager::Load(const std::string& _Name, const std::string& _Path)
{
	std::string UpName = GameEngineString::toupper(_Name);

	GameEngineFolderTexture* FindRes = Find(UpName);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineFolderTexture* NewRes = new GameEngineFolderTexture();
	NewRes->SetName(UpName);
	NewRes->Load(_Path);
	ResourcesMap.insert(std::map<std::string, GameEngineFolderTexture*>::value_type(UpName, NewRes));
	return NewRes;
}

GameEngineFolderTexture* GameEngineFolderTextureManager::Find(const std::string& _Name)
{
	std::string UpName = GameEngineString::toupper(_Name);

	std::map<std::string, GameEngineFolderTexture*>::iterator FindIter = ResourcesMap.find(UpName);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}