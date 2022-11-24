#pragma once
#include <GameEngine/GameEngineDevice.h>

// 설명 : 
class GameEngineRasterizer;
class GameEngineRasterizerManager
{
private:
	static GameEngineRasterizerManager* Inst;

public:
	static GameEngineRasterizerManager& GetInst()
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
	std::map<std::string, GameEngineRasterizer*> ResourcesMap;

public:
	// 직접 만들수 있다.
	GameEngineRasterizer* Create(const std::string& _Name, const D3D11_RASTERIZER_DESC& _RasterizerDesc);
	// 파일에서 로드
	GameEngineRasterizer* Load(const std::string& _Path);
	// 이름 직접 지정
	GameEngineRasterizer* Load(const std::string& _Name, const std::string& _Path);
	// 목록에서 찾는다.
	GameEngineRasterizer* Find(const std::string& _Name);

private:
	GameEngineRasterizerManager(); // default constructer 디폴트 생성자
	~GameEngineRasterizerManager(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineRasterizerManager(const GameEngineRasterizerManager& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineRasterizerManager(GameEngineRasterizerManager&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineRasterizerManager& operator=(const GameEngineRasterizerManager& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineRasterizerManager& operator=(const GameEngineRasterizerManager&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자
};




