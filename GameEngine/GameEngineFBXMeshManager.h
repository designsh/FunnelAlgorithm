#pragma once
#include <GameEngine/GameEngineDevice.h>
#include "GameEngineFBXMesh.h"

// ���� : 
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
	// ���Ͽ��� �ε�
	GameEngineFBXMesh* Load(const std::string& _Path);
	// �̸� ���� ����
	GameEngineFBXMesh* Load(const std::string& _Name, const std::string& _Path);
	// ��Ͽ��� ã�´�.


	GameEngineFBXMesh* LoadUser(const std::string& _Path);
	GameEngineFBXMesh* LoadUser(const std::string& _Name, const std::string& _Path);
	GameEngineFBXMesh* Find(const std::string& _Name);

	void DeletePath(const std::string& _Path);

	void Delete(const std::string& _Name);

private:
	GameEngineFBXMeshManager(); // default constructer ����Ʈ ������
	~GameEngineFBXMeshManager(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineFBXMeshManager(const GameEngineFBXMeshManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineFBXMeshManager(GameEngineFBXMeshManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineFBXMeshManager& operator=(const GameEngineFBXMeshManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineFBXMeshManager& operator=(const GameEngineFBXMeshManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
};




