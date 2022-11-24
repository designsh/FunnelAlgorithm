#pragma once
#include "GameEngineDevice.h"
#include <mutex>


// ���� : 
class GameEngineTexture;
class GameEngineTextureManager
{
private:
	static GameEngineTextureManager* Inst;
	static std::mutex ManagerLock;

public:
	static GameEngineTextureManager& GetInst()
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
	std::map<std::string, GameEngineTexture*> ResourcesMap;

public:
	// ���� ����� �ִ�.
	GameEngineTexture* Create(const std::string& _Name, ID3D11Texture2D* _Texture2D);

	// ���Ͽ��� �ε�
	GameEngineTexture* Load(const std::string& _Path);
	// �̸� ���� ����
	GameEngineTexture* Load(const std::string& _Name, const std::string& _Path);
	// ��Ͽ��� ã�´�.
	GameEngineTexture* Find(const std::string& _Name);

private:
	GameEngineTextureManager(); // default constructer ����Ʈ ������
	~GameEngineTextureManager(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineTextureManager(const GameEngineTextureManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineTextureManager(GameEngineTextureManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineTextureManager& operator=(const GameEngineTextureManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineTextureManager& operator=(const GameEngineTextureManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
};




