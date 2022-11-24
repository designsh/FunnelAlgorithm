#include "PreCompile.h"
#include "GameEngineFontManager.h"
#include "GameEngineFont.h"

GameEngineFontManager* GameEngineFontManager::Inst = new GameEngineFontManager();

GameEngineFontManager::GameEngineFontManager() // default constructer 디폴트 생성자
{

}

GameEngineFontManager::~GameEngineFontManager() // default destructer 디폴트 소멸자
{
	for (const std::pair<std::string, GameEngineFont*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineFontManager::GameEngineFontManager(GameEngineFontManager&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}
GameEngineFont* GameEngineFontManager::Load(const std::string& _Name)
{
	GameEngineFont* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineFont* NewRes = new GameEngineFont();
	NewRes->SetName(_Name);
	NewRes->Load(_Name);


	ResourcesMap.insert(std::map<std::string, GameEngineFont*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineFont* GameEngineFontManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineFont*>::iterator FindIter = ResourcesMap.find(_Name);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}