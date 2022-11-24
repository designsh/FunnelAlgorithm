#pragma once
#include "GameEngineDevice.h"

// 설명 : 
class GameEngineFolderTexture;
class GameEngineFolderTextureManager
{
private:
	static GameEngineFolderTextureManager* Inst;

public:
	static GameEngineFolderTextureManager& GetInst()
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
	std::map<std::string, GameEngineFolderTexture*> ResourcesMap;

public:
	GameEngineFolderTexture* Load(const std::string& _Path);

	// 이름 직접 지정
	GameEngineFolderTexture* Load(const std::string& _Name, const std::string& _Path);
	// 목록에서 찾는다.
	GameEngineFolderTexture* Find(const std::string& _Name);

private:
	GameEngineFolderTextureManager(); // default constructer 디폴트 생성자
	~GameEngineFolderTextureManager(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineFolderTextureManager(const GameEngineFolderTextureManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineFolderTextureManager(GameEngineFolderTextureManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineFolderTextureManager& operator=(const GameEngineFolderTextureManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineFolderTextureManager& operator=(const GameEngineFolderTextureManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};




