#include "PreCompile.h"
#include "GameEngineDepthStencilManager.h"
#include "GameEngineDepthStencil.h"

GameEngineDepthStencilManager* GameEngineDepthStencilManager::Inst = new GameEngineDepthStencilManager();

GameEngineDepthStencilManager::GameEngineDepthStencilManager() // default constructer 디폴트 생성자
{

}

GameEngineDepthStencilManager::~GameEngineDepthStencilManager() // default destructer 디폴트 소멸자
{
	for (const std::pair<std::string, GameEngineDepthStencil*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineDepthStencilManager::GameEngineDepthStencilManager(GameEngineDepthStencilManager&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}



GameEngineDepthStencil* GameEngineDepthStencilManager::Create(const std::string& _Name, const D3D11_DEPTH_STENCIL_DESC& DepthInfo)
{
	GameEngineDepthStencil* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Create");
	}


	GameEngineDepthStencil* NewRes = new GameEngineDepthStencil();
	NewRes->SetName(_Name);
	NewRes->Create(DepthInfo);

	// 그리고 뭘할거냐?

	ResourcesMap.insert(std::map<std::string, GameEngineDepthStencil*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineDepthStencil* GameEngineDepthStencilManager::Load(const std::string& _Path)
{
	return Load(GameEnginePath::GetFileName(_Path), _Path);
}

GameEngineDepthStencil* GameEngineDepthStencilManager::Load(const std::string& _Name, const std::string& _Path)
{
	GameEngineDepthStencil* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Load");
	}

	GameEngineDepthStencil* NewRes = new GameEngineDepthStencil();
	NewRes->SetName(_Name);


	ResourcesMap.insert(std::map<std::string, GameEngineDepthStencil*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineDepthStencil* GameEngineDepthStencilManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineDepthStencil*>::iterator FindIter = ResourcesMap.find(_Name);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}