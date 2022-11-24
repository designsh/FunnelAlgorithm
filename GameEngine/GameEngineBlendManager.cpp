#include "PreCompile.h"
#include "GameEngineBlendManager.h"
#include "GameEngineBlend.h"

GameEngineBlendManager* GameEngineBlendManager::Inst = new GameEngineBlendManager();

GameEngineBlendManager::GameEngineBlendManager() // default constructer ����Ʈ ������
{

}

GameEngineBlendManager::~GameEngineBlendManager() // default destructer ����Ʈ �Ҹ���
{
	for (const std::pair<std::string, GameEngineBlend*>& Res : ResourcesMap)
	{
		if (nullptr != Res.second)
		{
			delete Res.second;
		}
	}

	ResourcesMap.clear();
}

GameEngineBlendManager::GameEngineBlendManager(GameEngineBlendManager&& _other) noexcept  // default RValue Copy constructer ����Ʈ RValue ���������
{

}



GameEngineBlend* GameEngineBlendManager::Create(const std::string& _Name, const D3D11_BLEND_DESC& _Info, float4 _Fector /*= float4::ONE*/, unsigned int _Mask /*= 0xfffffff*/)
{
	GameEngineBlend* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		GameEngineDebug::MsgBoxError(_Name + " Is Overlap Create");
	}


	GameEngineBlend* NewRes = new GameEngineBlend();
	NewRes->SetName(_Name);
	NewRes->Create(_Info, _Fector, _Mask);

	// �׸��� ���Ұų�?

	ResourcesMap.insert(std::map<std::string, GameEngineBlend*>::value_type(_Name, NewRes));
	return NewRes;
}

GameEngineBlend* GameEngineBlendManager::Find(const std::string& _Name)
{
	std::map<std::string, GameEngineBlend*>::iterator FindIter = ResourcesMap.find(_Name);

	if (FindIter != ResourcesMap.end())
	{
		return FindIter->second;
	}

	return nullptr;
}

GameEngineBlend* GameEngineBlendManager::CreateAndFind(const std::string& _Name, const D3D11_BLEND_DESC& _Info, float4 _Fector /*= float4::ONE*/, unsigned int _Mask /*= 0xfffffff*/)
{
	GameEngineBlend* FindRes = Find(_Name);

	if (nullptr != FindRes)
	{
		return FindRes;
	}

	GameEngineBlend* NewRes = new GameEngineBlend();
	NewRes->SetName(_Name);
	NewRes->Create(_Info, _Fector, _Mask);

	ResourcesMap.insert(std::map<std::string, GameEngineBlend*>::value_type(_Name, NewRes));

	return NewRes;
}