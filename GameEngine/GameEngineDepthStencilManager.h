#pragma once
#include "GameEngineDevice.h"

// 설명 : 
class GameEngineDepthStencil;
class GameEngineDepthStencilManager
{
private:
	static GameEngineDepthStencilManager* Inst;

public:
	static GameEngineDepthStencilManager& GetInst()
	{
		return *Inst;
	}

	static void Destroy()
	{
		if (nullptr != Inst)
		{
			delete Inst;
			Inst = nullptr;
		}
	}

private:	// member Var
	std::map<std::string, GameEngineDepthStencil*> ResourcesMap;

public:
	// 직접 만들수 있다.
	GameEngineDepthStencil* Create(const std::string& _Name, const D3D11_DEPTH_STENCIL_DESC& DepthInfo);
	// 파일에서 로드
	GameEngineDepthStencil* Load(const std::string& _Path);
	// 이름 직접 지정
	GameEngineDepthStencil* Load(const std::string& _Name, const std::string& _Path);
	// 목록에서 찾는다.
	GameEngineDepthStencil* Find(const std::string& _Name);

private:
	GameEngineDepthStencilManager(); // default constructer 디폴트 생성자
	~GameEngineDepthStencilManager(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineDepthStencilManager(const GameEngineDepthStencilManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineDepthStencilManager(GameEngineDepthStencilManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineDepthStencilManager& operator=(const GameEngineDepthStencilManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineDepthStencilManager& operator=(const GameEngineDepthStencilManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};




