#pragma once
#include <GameEngine/GameEngineDevice.h>
#include "GameEngineFBXMesh.h"

// 설명 : 
class GameEngineFBXMesh;
class GameEngineFBXMeshManager
{
	friend class GameEngineFBXWindow;

private:
	static GameEngineFBXMeshManager* Inst;

public:
	static GameEngineFBXMeshManager& GetInst()
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
	std::map<std::string, GameEngineFBXMesh*> ResourcesMap;

public:
	// 파일에서 로드
	GameEngineFBXMesh* Load(const std::string& _Path);
	// 이름 직접 지정
	GameEngineFBXMesh* Load(const std::string& _Name, const std::string& _Path);
	// 목록에서 찾는다.


	GameEngineFBXMesh* LoadUser(const std::string& _Path);
	GameEngineFBXMesh* LoadUser(const std::string& _Name, const std::string& _Path);
	GameEngineFBXMesh* Find(const std::string& _Name);

	void DeletePath(const std::string& _Path);

	void Delete(const std::string& _Name);

private:
	GameEngineFBXMeshManager(); // default constructer 디폴트 생성자
	~GameEngineFBXMeshManager(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineFBXMeshManager(const GameEngineFBXMeshManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineFBXMeshManager(GameEngineFBXMeshManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineFBXMeshManager& operator=(const GameEngineFBXMeshManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineFBXMeshManager& operator=(const GameEngineFBXMeshManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};




