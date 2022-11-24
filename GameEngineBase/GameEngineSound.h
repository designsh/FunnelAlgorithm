#pragma once
#include "GameEngineSoundManager.h"
#include "GameEngineObjectNameBase.h"

// �з� :
// �뵵 :
// ���� :
class GameEngineSoundPlayer;
class GameEngineSound : public GameEngineObjectNameBase
{
	friend GameEngineSoundManager;
	friend GameEngineSoundPlayer;

private:	// member Var
	FMOD::Sound* sound_;

public:
	bool Load(const std::string& _Path);


private:		
	GameEngineSound(); // default constructer ����Ʈ ������
	~GameEngineSound(); // default destructer ����Ʈ �Ҹ���

public:		// delete constructer
	GameEngineSound(const GameEngineSound& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineSound(GameEngineSound&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

public:		//delete operator
	GameEngineSound& operator=(const GameEngineSound& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineSound& operator=(const GameEngineSound&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:		//member Func
};

