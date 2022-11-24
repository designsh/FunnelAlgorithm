#pragma once
#include "FMOD/fmod.hpp"
#include <list>
#include <map>
#include <string>

#ifdef _DEBUG
#pragma comment(lib, "fmodL_vc.lib") // Release용 빌드된 코드파일
#else
#pragma comment(lib, "fmod_vc.lib") // Release용 빌드된 코드파일
#endif

// 이런 사운드 이미지 리소스 폰트 우리가 로드하거나 
// 구현할줄 몰르거나 너무 귀찮아서 사용하는 라이브러리들

// 단계가 다음과 같다.
// 이니셜라이즈 함수를 찾는다. <= 대부분 그 라이브러의 전체 총괄 시스템용 객체가 존재한다.
// 릴리즈함수에요.
// 로드 함수를 찾아야한다.
// 그다음에 랜더링이나 재생함수를 찾아야 합니다.

// 분류 :
// 용도 :
// 설명 :
class GameEngineSound;
class GameEngineSoundPlayer;
class GameEngineSoundManager
{
private:
	static GameEngineSoundManager* Inst;

public:
	static GameEngineSoundManager& GetInst()
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

public:
	friend GameEngineSound;
	friend GameEngineSoundPlayer;

private:	// member Var
	FMOD::System* soundSystem_; // Fmod가 제공해주는 인터페이스
	// 사운드파일하나하나가 아니라
	// fmod를 사용할수 있는지 확인해주고 사운드를 로드할수 있게 해주는 기본 인터페이스입니다.
	// 이녀석이 먼저 제대로 만들어질수 있는 환경이어야 사운드를 사용할수 있습니다.
	std::map<std::string, GameEngineSound*> allLoadSound_;

	std::list<GameEngineSoundPlayer*> allSoundPlayer_;

private:
	GameEngineSound* FindSound(const std::string& _name);

public:
	void Initialize();

	void Load(const std::string& _path);

	void Load(const std::string& _name, const std::string& _path);
	void PlaySoundOneShot(const std::string& _name);
	GameEngineSoundPlayer* CreateSoundPlayer();

public:
	void SoundUpdate();

private:		
	GameEngineSoundManager(); // default constructer 디폴트 생성자
	~GameEngineSoundManager(); // default destructer 디폴트 소멸자

public:		// delete constructer
	GameEngineSoundManager(const GameEngineSoundManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineSoundManager(GameEngineSoundManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

public:		//delete operator
	GameEngineSoundManager& operator=(const GameEngineSoundManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineSoundManager& operator=(const GameEngineSoundManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:		//member Func
};

