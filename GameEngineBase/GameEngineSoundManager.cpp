#include "PreCompile.h"
#include "GameEngineSound.h"
#include "GameEngineDebug.h"
#include "GameEngineSound.h"
#include "GameEngineSoundPlayer.h"
#include "GameEnginePath.h"


GameEngineSoundManager* GameEngineSoundManager::Inst = new GameEngineSoundManager();

// Static Var
// Static Func

// constructer destructer
GameEngineSoundManager::GameEngineSoundManager()
{
}

GameEngineSoundManager::~GameEngineSoundManager()
{
	{
		std::list<GameEngineSoundPlayer*>::iterator StartIter = allSoundPlayer_.begin();
		std::list<GameEngineSoundPlayer*>::iterator EndIter = allSoundPlayer_.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if (nullptr != *StartIter)
			{
				delete *StartIter;
			}
		}
		allSoundPlayer_.clear();
	}

	{
		std::map<std::string, GameEngineSound*>::iterator StartIter = allLoadSound_.begin();
		std::map<std::string, GameEngineSound*>::iterator EndIter = allLoadSound_.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			if (nullptr != StartIter->second)
			{
				delete StartIter->second;
				StartIter->second = nullptr;
			}
		}
		allLoadSound_.clear();
	}


	if (nullptr != soundSystem_)
	{
		soundSystem_->release();
		soundSystem_ = nullptr;
	}
}

GameEngineSoundManager::GameEngineSoundManager(GameEngineSoundManager&& _other) noexcept
{

}

GameEngineSound* GameEngineSoundManager::FindSound(const std::string& _name)
{
	std::map<std::string, GameEngineSound*>::iterator FindIter = allLoadSound_.find(_name);

	if (FindIter == allLoadSound_.end())
	{
		return nullptr;
	}

	return FindIter->second;
}

void GameEngineSoundManager::SoundUpdate() 
{
	if (nullptr == soundSystem_)
	{
		GameEngineDebug::MsgBoxError("SoundSystem Is null");
		return;
	}

	soundSystem_->update();
}

GameEngineSoundPlayer* GameEngineSoundManager::CreateSoundPlayer()
{
	GameEngineSoundPlayer* NewSoundplayer = new GameEngineSoundPlayer();

	allSoundPlayer_.push_back(NewSoundplayer);

	return  NewSoundplayer;
}

void GameEngineSoundManager::Load(const std::string& _path) 
{
	Load(GameEnginePath::GetFileName(_path), _path);
}

void GameEngineSoundManager::Load(const std::string& _name, const std::string& _path)
{
	if (nullptr != FindSound(_name))
	{
		GameEngineDebug::MsgBoxError("Sound Load overlap error");
		return;
	}

	GameEngineSound* newLoadSound = new GameEngineSound();

	if (false == newLoadSound->Load(_path))
	{
		GameEngineDebug::MsgBoxError("Sound Load Error");
		delete newLoadSound;
		return;
	}

	allLoadSound_.insert(
		std::map<std::string, GameEngineSound*>::value_type(_name, newLoadSound));
}

void GameEngineSoundManager::PlaySoundOneShot(const std::string& _name) 
{
	GameEngineSound* SoundPtr = FindSound(_name);

	if (nullptr == SoundPtr)
	{
		GameEngineDebug::MsgBoxError("PlaySound Error");
		return;
	}

	soundSystem_->playSound(SoundPtr->sound_, nullptr, false, nullptr);
}

//member Func

void GameEngineSoundManager::Initialize()
{
	// 내부코드에서 NEW를 할 가능성이 매우 다분해.
	FMOD::System_Create(&soundSystem_);

	if (nullptr == soundSystem_)
	{
		GameEngineDebug::MsgBoxError("sound system create Error");
		return;
	}

	// 동시에 32개 개수인지 사운드 채널의 의미인지를 잘 모르고 있습니다.
	// 32채널을 재생할수 있다는 의미인데 선생님도 잘 모릅니다.
	if (FMOD_OK != soundSystem_->init(32, FMOD_DEFAULT, nullptr))
	{
		GameEngineDebug::MsgBoxError("sound system init Error");
		return;
	}
}
