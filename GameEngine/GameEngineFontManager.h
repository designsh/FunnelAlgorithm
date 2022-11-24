#pragma once

// 설명 : 
class GameEngineFont;
class GameEngineFontManager
{
private:
	static GameEngineFontManager* Inst;

public:
	static GameEngineFontManager& GetInst()
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
	std::map<std::string, GameEngineFont*> ResourcesMap;

public:
	GameEngineFont* Load(const std::string& _Path);
	// 목록에서 찾는다.
	GameEngineFont* Find(const std::string& _Name);

private:
	GameEngineFontManager(); // default constructer 디폴트 생성자
	~GameEngineFontManager(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineFontManager(const GameEngineFontManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineFontManager(GameEngineFontManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineFontManager& operator=(const GameEngineFontManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineFontManager& operator=(const GameEngineFontManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};




