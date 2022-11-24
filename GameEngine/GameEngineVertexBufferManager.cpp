#include "PreCompile.h"
#include "GameEngineVertexBufferManager.h"
#include "GameEngineVertexBuffer.h"

GameEngineVertexBufferManager* GameEngineVertexBufferManager::Inst = new GameEngineVertexBufferManager();

GameEngineVertexBufferManager::GameEngineVertexBufferManager() // default constructer 디폴트 생성자
{

}

GameEngineVertexBufferManager::~GameEngineVertexBufferManager() // default destructer 디폴트 소멸자
{
	for (const std::pair<std::string, GameEngineVertexBuffer*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineVertexBufferManager::GameEngineVertexBufferManager(GameEngineVertexBufferManager&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}


GameEngineVertexBuffer* GameEngineVertexBufferManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

GameEngineVertexBuffer* GameEngineVertexBufferManager::Load(const std::string& _Name, const std::string& _Path)
{
	GameEngineVertexBuffer* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineVertexBuffer* NewRes = new GameEngineVertexBuffer();
	NewRes->SetName(_Name);


	ResourcesMap.insert(std::map<std::string, GameEngineVertexBuffer*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineVertexBuffer* GameEngineVertexBufferManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineVertexBuffer*>::iterator FindIter = ResourcesMap.find(_Name);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}