#pragma once
#include "GameEngineDevice.h"

// 설명 : 
class GameEngineConstantBuffer;
class GameEngineConstantBufferManager
{
private:
	static GameEngineConstantBufferManager* Inst;

public:
	static GameEngineConstantBufferManager& GetInst()
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
	std::map<std::string, GameEngineConstantBuffer*> ResourcesMap;

public:
	// 직접 만들수 있다.
	GameEngineConstantBuffer* Create(const std::string& _Name, const D3D11_SHADER_BUFFER_DESC& _BufferDesc, ID3D11ShaderReflectionConstantBuffer* _VarInfo);

	GameEngineConstantBuffer* CreateAndFind(const std::string& _Name, const D3D11_SHADER_BUFFER_DESC& _BufferDesc, ID3D11ShaderReflectionConstantBuffer* _VarInfo);

	// 파일에서 로드
	GameEngineConstantBuffer* Load(const std::string& _Path);
	// 이름 직접 지정
	GameEngineConstantBuffer* Load(const std::string& _Name, const std::string& _Path);
	// 목록에서 찾는다.
	GameEngineConstantBuffer* Find(const std::string& _Name);

private:
	GameEngineConstantBufferManager(); // default constructer 디폴트 생성자
	~GameEngineConstantBufferManager(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineConstantBufferManager(const GameEngineConstantBufferManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineConstantBufferManager(GameEngineConstantBufferManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineConstantBufferManager& operator=(const GameEngineConstantBufferManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineConstantBufferManager& operator=(const GameEngineConstantBufferManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};




