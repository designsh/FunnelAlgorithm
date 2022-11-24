#pragma once
#include <GameEngineBase/GameEngineMath.h>

#include <d3d11_4.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// 다이렉트 x는 프로세스마다 GUIID를 부여서해 X프로세스가 디바이스를 이용하려고 하는지 알아서 처리해줘요.
#pragma comment(lib, "dxguid")

class GameEngineDepthBuffer;
class GameEngineRenderTarget;
// 분류 : 
// 용도 : 
// 설명 : 그래픽카드에 접근할수 있는 포인터
class GameEngineDevice
{
private:	// member Var
	static GameEngineDevice* Inst;

public:
	static GameEngineDevice& GetInst()
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

private:
	static ID3D11Device* Device_;
	// 그래픽카드에 뭔가 공간을 할당하려고 하면 디바이스를 거쳐야 합니다.
	static ID3D11DeviceContext* Context_;
	// 그래픽카드에게 뭔가를 그리라고 명령하거나 그 그리는데 필요한 옵션을 세팅하려면 컨텍스트를 거쳐야 합니다.
	static IDXGISwapChain* SwapChain_;

	static GameEngineRenderTarget* BackBufferTarget_;
	
	// static GameEngineDepthBuffer* DepthBuffer_;

public:
	static GameEngineRenderTarget* GetBackBufferTarget();
	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetContext();

	// 메모리로서의 텍스처의 역할
	// ID3D11Texture2D* BackBufferTexture_;

	// 랜더링 파이프라인에 세팅하기 위한 역할
	// ID3D11RenderTargetView* BackBufferViewTexture_;

public:
	void Initialize();
	void CreateSwapChain();

private:


public:
	GameEngineDevice(); // default constructer 디폴트 생성자
	~GameEngineDevice(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineDevice(const GameEngineDevice& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineDevice(GameEngineDevice&& _other) = delete; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineDevice& operator=(const GameEngineDevice& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineDevice& operator=(const GameEngineDevice&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	static void ShaderReset();
	static void ResourceReset();
	static void RenderStart();
	static void RenderEnd();
};

