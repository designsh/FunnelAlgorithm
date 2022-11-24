#pragma once
#include <Windows.h>
#include <GameEngine/GameEngineDevice.h>

// 설명 : 
class GameEngineIndexBuffer;
class GameEngineIndexBufferManager
{
private:
	static GameEngineIndexBufferManager* Inst;

public:
	static GameEngineIndexBufferManager& GetInst()
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
	std::map<std::string, GameEngineIndexBuffer*> ResourcesMap;

public:
	// 직접 만들수 있다.
	GameEngineIndexBuffer* Create(const std::string& _Name, const std::vector<unsigned int>& _Index, D3D11_USAGE _Usage);
	// 파일에서 로드
	GameEngineIndexBuffer* Load(const std::string& _Path);
	// 이름 직접 지정
	GameEngineIndexBuffer* Load(const std::string& _Name, const std::string& _Path);
	// 목록에서 찾는다.
	GameEngineIndexBuffer* Find(const std::string& _Name);

private:
	GameEngineIndexBufferManager(); // default constructer 디폴트 생성자
	~GameEngineIndexBufferManager(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineIndexBufferManager(const GameEngineIndexBufferManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineIndexBufferManager(GameEngineIndexBufferManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineIndexBufferManager& operator=(const GameEngineIndexBufferManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineIndexBufferManager& operator=(const GameEngineIndexBufferManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};




