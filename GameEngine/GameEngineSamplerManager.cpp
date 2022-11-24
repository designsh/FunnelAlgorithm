#include "PreCompile.h"
#include "GameEngineSamplerManager.h"
#include "GameEngineSampler.h"

GameEngineSamplerManager* GameEngineSamplerManager::Inst = new GameEngineSamplerManager();

GameEngineSamplerManager::GameEngineSamplerManager() // default constructer 디폴트 생성자
{

}

GameEngineSamplerManager::~GameEngineSamplerManager() // default destructer 디폴트 소멸자
{
	for (const std::pair<std::string, GameEngineSampler*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineSamplerManager::GameEngineSamplerManager(GameEngineSamplerManager&& _other) noexcept  // default RValue Copy constructer 디폴트 RValue 복사생성자
{

}



GameEngineSampler* GameEngineSamplerManager::Create(const std::string& _Name, const D3D11_SAMPLER_DESC& _Info)
{
	std::string UpperName = GameEngineString::toupper(_Name);


	GameEngineSampler* FindRes = Find(UpperName);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Create");
	}


	GameEngineSampler* NewRes = new GameEngineSampler();
	NewRes->SetName(UpperName);
	NewRes->Create(_Info);

	// 그리고 뭘할거냐?

	ResourcesMap.insert(std::map<std::string, GameEngineSampler*>::value_type(UpperName, NewRes));
	return NewRes;
}

GameEngineSampler* GameEngineSamplerManager::Find(const std::string& _Name)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	std::map<std::string, GameEngineSampler*>::iterator FindIter = ResourcesMap.find(UpperName);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}

GameEngineSampler* GameEngineSamplerManager::CreateAndFind(const std::string& _Name, const D3D11_SAMPLER_DESC& _Info)
{
	std::string UpperName = GameEngineString::toupper(_Name);

	GameEngineSampler* FindRes = Find(UpperName);

	if (nullptr != FindRes)
	{
		return FindRes;
	}

	GameEngineSampler* NewRes = new GameEngineSampler();
	NewRes->SetName(UpperName);
	NewRes->Create(_Info);

	ResourcesMap.insert(std::map<std::string, GameEngineSampler*>::value_type(UpperName, NewRes));

	return NewRes;
}