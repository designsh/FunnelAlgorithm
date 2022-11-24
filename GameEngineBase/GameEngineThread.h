#pragma once
#include "GameEngineObjectNameBase.h"
#include <thread>
#include <string>
#include <functional>

// ���� :
class GameEngineThread : public GameEngineObjectNameBase
{
public:
	// constrcuter destructer
	GameEngineThread();
	~GameEngineThread();

	// delete Function
	GameEngineThread(const GameEngineThread& _Other) = delete;
	GameEngineThread(GameEngineThread&& _Other) noexcept = delete;
	GameEngineThread& operator=(const GameEngineThread& _Other) = delete;
	GameEngineThread& operator=(GameEngineThread&& _Other) noexcept = delete;

	//static std::string GetThreadName()
	//{
	//	return ThreadName;
	//}


	void Start(std::string _ThreadName, std::function<void(GameEngineThread*)> _Function)
	{
		ThreadFunction = _Function;
		Thread = std::thread(GameServerThreadFunction, this, _ThreadName);
	}

	void Join()
	{
		Thread.join();
	}

protected:



private:
	std::thread Thread;
	std::function<void(GameEngineThread*)> ThreadFunction;



	static void GameServerThreadFunction(GameEngineThread* _Thread, std::string _Name);

	// �����帶�� ���������� ������ �˴ϴ�.
	// static thread_local std::string ThreadName;

};

