#pragma once
#include <GameEngineBase/GameEngineMath.h>

#include <d3d11_4.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// ���̷�Ʈ x�� ���μ������� GUIID�� �ο����� X���μ����� ����̽��� �̿��Ϸ��� �ϴ��� �˾Ƽ� ó�������.
#pragma comment(lib, "dxguid")

class GameEngineDepthBuffer;
class GameEngineRenderTarget;
// �з� : 
// �뵵 : 
// ���� : �׷���ī�忡 �����Ҽ� �ִ� ������
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
	// �׷���ī�忡 ���� ������ �Ҵ��Ϸ��� �ϸ� ����̽��� ���ľ� �մϴ�.
	static ID3D11DeviceContext* Context_;
	// �׷���ī�忡�� ������ �׸���� ����ϰų� �� �׸��µ� �ʿ��� �ɼ��� �����Ϸ��� ���ؽ�Ʈ�� ���ľ� �մϴ�.
	static IDXGISwapChain* SwapChain_;

	static GameEngineRenderTarget* BackBufferTarget_;
	
	// static GameEngineDepthBuffer* DepthBuffer_;

public:
	static GameEngineRenderTarget* GetBackBufferTarget();
	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetContext();

	// �޸𸮷μ��� �ؽ�ó�� ����
	// ID3D11Texture2D* BackBufferTexture_;

	// ������ ���������ο� �����ϱ� ���� ����
	// ID3D11RenderTargetView* BackBufferViewTexture_;

public:
	void Initialize();
	void CreateSwapChain();

private:


public:
	GameEngineDevice(); // default constructer ����Ʈ ������
	~GameEngineDevice(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineDevice(const GameEngineDevice& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineDevice(GameEngineDevice&& _other) = delete; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineDevice& operator=(const GameEngineDevice& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineDevice& operator=(const GameEngineDevice&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	static void ShaderReset();
	static void ResourceReset();
	static void RenderStart();
	static void RenderEnd();
};

