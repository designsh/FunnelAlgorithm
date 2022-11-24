#pragma once
#include "GameEngineDevice.h"

// ���� : 
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

	// �̸� ���� ����
	GameEngineFolderTexture* Load(const std::string& _Name, const std::string& _Path);
	// ��Ͽ��� ã�´�.
	GameEngineFolderTexture* Find(const std::string& _Name);

private:
	GameEngineFolderTextureManager(); // default constructer ����Ʈ ������
	~GameEngineFolderTextureManager(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineFolderTextureManager(const GameEngineFolderTextureManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineFolderTextureManager(GameEngineFolderTextureManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineFolderTextureManager& operator=(const GameEngineFolderTextureManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineFolderTextureManager& operator=(const GameEngineFolderTextureManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
};




