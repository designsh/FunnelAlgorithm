#pragma once

// 설명 : 
class GameEngineVertexShader;
class GameEngineVertexShaderManager
{
private:
	static GameEngineVertexShaderManager* Inst;

public:
	static GameEngineVertexShaderManager& GetInst()
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
	std::map<std::string, GameEngineVertexShader*> ResourcesMap;

public:
	// 직접 만들수 있다.
	GameEngineVertexShader* Create(const std::string& _Name,
		const std::string& _ShaderCode,
		UINT _VersionHigh = 5,
		UINT _VersionLow = 0
	) 
	{
		return Create(_Name, _ShaderCode, _Name, _VersionHigh, _VersionLow);
	}


	GameEngineVertexShader* Create(const std::string& _Name,
		const std::string& _ShaderCode,
		const std::string& _EntryPoint,
		UINT _VersionHigh = 5,
		UINT _VersionLow = 0
		);
	// 파일에서 로드
	GameEngineVertexShader* Load(const std::string& _Path
		, const std::string& _EntryPoint
		, UINT _VersionHigh = 5
		, UINT _VersionLow = 0
	);
	// 이름 직접 지정
	GameEngineVertexShader* Load(const std::string& _Name, const std::string& _Path, const std::string& _EntryPoint
		, UINT _VersionHigh = 5
		, UINT _VersionLow = 0
	);
	// 목록에서 찾는다.
	GameEngineVertexShader* Find(const std::string& _Name);

private:
	GameEngineVertexShaderManager(); // default constructer 디폴트 생성자
	~GameEngineVertexShaderManager(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineVertexShaderManager(const GameEngineVertexShaderManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineVertexShaderManager(GameEngineVertexShaderManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineVertexShaderManager& operator=(const GameEngineVertexShaderManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineVertexShaderManager& operator=(const GameEngineVertexShaderManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};




