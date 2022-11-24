#pragma once
#include "GameEngineDevice.h"

// 설명 : 
class GameEngineDepthBuffer;
class GameEngineDepthBufferManager
{
private:
	static GameEngineDepthBufferManager* Inst;

public:
	static GameEngineDepthBufferManager& GetInst()
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
	std::map<std::string, GameEngineDepthBuffer*> ResourcesMap;

public:
	// 직접 만들수 있다.
	GameEngineDepthBuffer* Create(const std::string& _Name, float4 _Size);
	// 파일에서 로드
	GameEngineDepthBuffer* Load(const std::string& _Path);
	// 이름 직접 지정
	GameEngineDepthBuffer* Load(const std::string& _Name, const std::string& _Path);
	// 목록에서 찾는다.
	GameEngineDepthBuffer* Find(const std::string& _Name);

private:
	GameEngineDepthBufferManager(); // default constructer 디폴트 생성자
	~GameEngineDepthBufferManager(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineDepthBufferManager(const GameEngineDepthBufferManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineDepthBufferManager(GameEngineDepthBufferManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineDepthBufferManager& operator=(const GameEngineDepthBufferManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineDepthBufferManager& operator=(const GameEngineDepthBufferManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};




