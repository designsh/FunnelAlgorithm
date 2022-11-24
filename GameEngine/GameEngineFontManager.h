#pragma once

// ���� : 
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
	// ��Ͽ��� ã�´�.
	GameEngineFont* Find(const std::string& _Name);

private:
	GameEngineFontManager(); // default constructer ����Ʈ ������
	~GameEngineFontManager(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineFontManager(const GameEngineFontManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineFontManager(GameEngineFontManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineFontManager& operator=(const GameEngineFontManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineFontManager& operator=(const GameEngineFontManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
};




