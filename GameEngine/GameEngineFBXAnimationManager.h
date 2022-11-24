#pragma once
#include <GameEngine/GameEngineDevice.h>
#include "GameEngineFBXAnimation.h"

// 설명 : 
class GameEngineFBXAnimation;
class GameEngineFBXAnimationManager
{
	friend class GameEngineFBXWindow;

private:
	static GameEngineFBXAnimationManager* Inst;

public:
	static GameEngineFBXAnimationManager& GetInst()
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
	std::map<std::string, GameEngineFBXAnimation*> ResourcesMap;

public:
	GameEngineFBXAnimation* LoadUser(const std::string& _Path);
	GameEngineFBXAnimation* LoadUser(const std::string& _Name, const std::string& _Path);

	// 파일에서 로드
	GameEngineFBXAnimation* Load(const std::string& _Path);
	// 이름 직접 지정
	GameEngineFBXAnimation* Load(const std::string& _Name, const std::string& _Path);
	// 목록에서 찾는다.
	GameEngineFBXAnimation* Find(const std::string& _Name);

	void DeletePath(const std::string& _Path);

	void Delete(const std::string& _Name);


private:
	GameEngineFBXAnimationManager(); // default constructer 디폴트 생성자
	~GameEngineFBXAnimationManager(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineFBXAnimationManager(const GameEngineFBXAnimationManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineFBXAnimationManager(GameEngineFBXAnimationManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineFBXAnimationManager& operator=(const GameEngineFBXAnimationManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineFBXAnimationManager& operator=(const GameEngineFBXAnimationManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};




