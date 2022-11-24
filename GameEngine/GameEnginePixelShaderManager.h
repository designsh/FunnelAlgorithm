#pragma once

// ���� : 
class GameEnginePixelShader;
class GameEnginePixelShaderManager
{
private:
	static GameEnginePixelShaderManager* Inst;

public:
	static GameEnginePixelShaderManager& GetInst()
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
	std::map<std::string, GameEnginePixelShader*> ResourcesMap;

public:
	// ���� ����� �ִ�.
	GameEnginePixelShader* Create(const std::string& _Name,
		const std::string& _ShaderCode,
		UINT _VersionHigh = 5,
		UINT _VersionLow = 0
	)
	{
		return Create(_Name, _ShaderCode, _Name, _VersionHigh, _VersionLow);
	}


	GameEnginePixelShader* Create(const std::string& _Name,
		const std::string& _ShaderCode,
		const std::string& _EntryPoint,
		UINT _VersionHigh = 5,
		UINT _VersionLow = 0
	);
	// ���Ͽ��� �ε�
	GameEnginePixelShader* Load(const std::string& _Path
		, const std::string& _EntryPoint
		, UINT _VersionHigh = 5
		, UINT _VersionLow = 0
	);
	// �̸� ���� ����
	GameEnginePixelShader* Load(const std::string& _Name, const std::string& _Path, const std::string& _EntryPoint
		, UINT _VersionHigh = 5
		, UINT _VersionLow = 0
	);
	// ��Ͽ��� ã�´�.
	GameEnginePixelShader* Find(const std::string& _Name);

private:
	GameEnginePixelShaderManager(); // default constructer ����Ʈ ������
	~GameEnginePixelShaderManager(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEnginePixelShaderManager(const GameEnginePixelShaderManager& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEnginePixelShaderManager(GameEnginePixelShaderManager&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEnginePixelShaderManager& operator=(const GameEnginePixelShaderManager& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEnginePixelShaderManager& operator=(const GameEnginePixelShaderManager&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����
};




