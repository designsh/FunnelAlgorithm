#include "PreCompile.h"
#include "GameEngineFBXMeshManager.h"
#include "GameEngineFBXMesh.h"

GameEngineFBXMeshManager* GameEngineFBXMeshManager::Inst = new GameEngineFBXMeshManager();

GameEngineFBXMeshManager::GameEngineFBXMeshManager() // default constructer 디폴트 생성자
{

}

GameEngineFBXMeshManager::~GameEngineFBXMeshManager() // default destructer 디폴트 소멸자
{
	for (const std::pair<std::string, GameEngineFBXMesh*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineFBXMeshManager::GameEngineFBXMeshManager(GameEngineFBXMeshManager&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}

GameEngineFBXMesh* GameEngineFBXMeshManager::LoadUser(const std::string& _Path)
{
	return LoadUser(GameEnginePath::GetFileName(_Path), _Path);
}
GameEngineFBXMesh* GameEngineFBXMeshManager::LoadUser(const std::string& _Name, const std::string& _Path)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineFBXMesh* FindRes = Find(UpperName);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineFBXMesh* NewRes = new GameEngineFBXMesh();
	NewRes->SetName(UpperName);
	NewRes->UserLoad(_Path);

	ResourcesMap.insert(std::map<std::string, GameEngineFBXMesh*>::value_type(UpperName, NewRes));
	return NewRes;

}

GameEngineFBXMesh* GameEngineFBXMeshManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

void GameEngineFBXMeshManager::DeletePath(const std::string& _Path)
{
	return Delete(GameEnginePath::GetFileName(_Path));
}

void GameEngineFBXMeshManager::Delete(const std::string& _Name)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineFBXMesh* FindRes = Find(UpperName);

	if (nullptr == FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + "는 존재하지 않는 매쉬 입니다 삭제할수 없습니다.");
		return;
	}

	delete FindRes;

	ResourcesMap.erase(UpperName);
	return;
}

GameEngineFBXMesh* GameEngineFBXMeshManager::Load(const std::string& _Name, const std::string& _Path)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineFBXMesh* FindRes = Find(UpperName);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineFBXMesh* NewRes = new GameEngineFBXMesh();
	NewRes->SetName(UpperName);
	NewRes->Load(_Path);

	ResourcesMap.insert(std::map<std::string, GameEngineFBXMesh*>::value_type(UpperName, NewRes));
	return NewRes;
}

GameEngineFBXMesh* GameEngineFBXMeshManager::Find(const std::string& _Name)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	std::map<std::string, GameEngineFBXMesh*>::iterator FindIter = ResourcesMap.find(UpperName);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}