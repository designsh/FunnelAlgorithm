#include "PreCompile.h"
#include "GameEngineDepthBufferManager.h"
#include "GameEngineDepthBuffer.h"

GameEngineDepthBufferManager* GameEngineDepthBufferManager::Inst = new GameEngineDepthBufferManager();

GameEngineDepthBufferManager::GameEngineDepthBufferManager() // default constructer 디폴트 생성자
{

}

GameEngineDepthBufferManager::~GameEngineDepthBufferManager() // default destructer 디폴트 소멸자
{
	for (const std::pair<std::string, GameEngineDepthBuffer*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineDepthBufferManager::GameEngineDepthBufferManager(GameEngineDepthBufferManager&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}



GameEngineDepthBuffer* GameEngineDepthBufferManager::Create(const std::string& _Name, float4 _Size)
{
	GameEngineDepthBuffer* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Create");
	}


	GameEngineDepthBuffer* NewRes = new GameEngineDepthBuffer();
	NewRes->SetName(_Name);
	NewRes->Create(_Size);

	// 그리고 뭘할거냐?

	ResourcesMap.insert(std::map<std::string, GameEngineDepthBuffer*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineDepthBuffer* GameEngineDepthBufferManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

GameEngineDepthBuffer* GameEngineDepthBufferManager::Load(const std::string& _Name, const std::string& _Path)
{
	GameEngineDepthBuffer* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineDepthBuffer* NewRes = new GameEngineDepthBuffer();
	NewRes->SetName(_Name);


	ResourcesMap.insert(std::map<std::string, GameEngineDepthBuffer*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineDepthBuffer* GameEngineDepthBufferManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineDepthBuffer*>::iterator FindIter = ResourcesMap.find(_Name);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}
