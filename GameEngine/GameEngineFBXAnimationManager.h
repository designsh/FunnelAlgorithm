#pragma once
#include <GameEngine/GameEngineDevice.h>
#include "GameEngineFBXAnimation.h"

// ���� : 
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

	// ���Ͽ��� �ε�
	GameEngineFBXAnimation* Load(const std::string& _Path);
	// �̸� ���� ����
	GameEngineFBXAnimation* Load(const std::string& _Name, const std::string& _Path);
	// ��Ͽ��� ã�´�.
	GameEngineFBXAnimation* Find(const std::string& _Name);

	void DeletePath(const std::string& _Path);

	void Delete(const std::string& _Name);


private:
	GameEngineFBXAnimationManager(); // default constructer ����Ʈ ������
	~GameEngineFBXAnimationManager(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineFBXAnimationManager(const GameEngineFBXAnimationManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineFBXAnimationManager(GameEngineFBXAnimationManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineFBXAnimationManager& operator=(const GameEngineFBXAnimationManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineFBXAnimationManager& operator=(const GameEngineFBXAnimationManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
};




