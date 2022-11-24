#pragma once
#include "GameEngineDevice.h"

// ���� : 
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
	// ���� ����� �ִ�.
	GameEngineDepthBuffer* Create(const std::string& _Name, float4 _Size);
	// ���Ͽ��� �ε�
	GameEngineDepthBuffer* Load(const std::string& _Path);
	// �̸� ���� ����
	GameEngineDepthBuffer* Load(const std::string& _Name, const std::string& _Path);
	// ��Ͽ��� ã�´�.
	GameEngineDepthBuffer* Find(const std::string& _Name);

private:
	GameEngineDepthBufferManager(); // default constructer ����Ʈ ������
	~GameEngineDepthBufferManager(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineDepthBufferManager(const GameEngineDepthBufferManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineDepthBufferManager(GameEngineDepthBufferManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineDepthBufferManager& operator=(const GameEngineDepthBufferManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineDepthBufferManager& operator=(const GameEngineDepthBufferManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
};




