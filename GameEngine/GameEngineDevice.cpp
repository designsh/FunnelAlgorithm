#include "PreCompile.h"
#include "GameEngineDevice.h"
#include "GameEngineWindow.h"
#include "GameEngineTextureManager.h"
#include "GameEngineRenderTargetManager.h"
#include "GameEngineDepthBufferManager.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineDepthBuffer.h"

GameEngineDevice* GameEngineDevice::Inst = new GameEngineDevice();

GameEngineRenderTarget* GameEngineDevice::BackBufferTarget_ = nullptr;
ID3D11Device* GameEngineDevice::Device_ = nullptr;
ID3D11DeviceContext* GameEngineDevice::Context_ = nullptr;
IDXGISwapChain* GameEngineDevice::SwapChain_ = nullptr;

GameEngineRenderTarget* GameEngineDevice::GetBackBufferTarget()
{
	return BackBufferTarget_;
}


ID3D11Device* GameEngineDevice::GetDevice() 
{
	return Device_;
}

ID3D11DeviceContext* GameEngineDevice::GetContext() 
{
	return Context_;
}

GameEngineDevice::GameEngineDevice() // default constructer ����Ʈ ������
{

}

GameEngineDevice::~GameEngineDevice() // default destructer ����Ʈ �Ҹ���
{
	if (nullptr != SwapChain_)
	{
		SwapChain_->Release();
		SwapChain_ = nullptr;
	}

	if (nullptr != Context_)
	{
		Context_->Release();
		Context_ = nullptr;
	}

	if (nullptr != Device_)
	{
		Device_->Release();
		Device_ = nullptr;
	}

}

void GameEngineDevice::Initialize() 
{
	if (nullptr == GameEngineWindow::GetInst().GetWindowHWND())
	{
		GameEngineDebug::MsgBoxError("Window Hwnd ERROR");
		return;
	}

	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	// ���� ����Ϸ��� ���̷�Ʈ ������ ���ΰ���?
	// ���̷�Ʈ 11 ����մϴ�.
	D3D_FEATURE_LEVEL eLV = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	// D3D_FEATURE_LEVEL eLVArr[3] = { D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_9_0;

	// D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE

	if (
		S_OK != D3D11CreateDevice(
			nullptr,  
			D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			iFlag,
			nullptr, // eLVArr
			0,
			D3D11_SDK_VERSION,
			&Device_,
			&eLV,
			&Context_)
		)
	{
		GameEngineDebug::MsgBoxError("D3D11 Create Device ERROR");
	}

	if (eLV != D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0)
	{
		GameEngineDebug::MsgBoxError("D3D11 Not FEATURE LEVEL 11 ERROR");
	}

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	// CreateSwapChain();
}

void GameEngineDevice::CreateSwapChain()
{
	if (0 >= GameEngineWindow::GetInst().GetSize().x && 
		0 >= GameEngineWindow::GetInst().GetSize().y)
	{
		GameEngineDebug::MsgBoxError("Screen Size Zero");
	}

	float4 ScreenSize = GameEngineWindow::GetInst().GetSize();

	DXGI_SWAP_CHAIN_DESC ScInfo = { 0, };

	// �׷���ī�忡 ���۴� ����־�� ����.
	ScInfo.BufferDesc.Width = ScreenSize.uix();
	ScInfo.BufferDesc.Height = ScreenSize.uiy();

	// ����Ϳ� �����ؼ� 
	// 1�ʿ�
	ScInfo.BufferDesc.RefreshRate.Denominator = 1;
	// 60������ ����۸� �����ض�.
	ScInfo.BufferDesc.RefreshRate.Numerator = 60;

	ScInfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ScInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	ScInfo.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ȭ�鿡 ���� ���ؼ��� �׷� �뵵�� ����ٴ°� �˷���� �ϴµ�
	ScInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;

	ScInfo.SampleDesc.Quality = 0;
	ScInfo.SampleDesc.Count = 1;

	ScInfo.OutputWindow = GameEngineWindow::GetInst().GetWindowHWND();

	ScInfo.BufferCount = 2;

	ScInfo.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// ��üȭ�� ��� ����
	ScInfo.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// ��üȭ�� ����.
	ScInfo.Windowed = true;

	// �׷���ī���� ȭ����¿� ���õ� ���ҽ��� �����Ҽ� �ִ� 
	// ��ɵ��� �����͸� ������
	// �� ��ɵ��� ���ؼ� ������� �ؽ�ó�� ���� ���� ���̴�.

	// ID3D11Device* != IDXGIDevice;
	// �׷���ī���� �޸𸮿� �����ڸ� �����Ѵ�..
	IDXGIDevice* pD = nullptr;
	IDXGIAdapter* pA = nullptr;
	IDXGIFactory* pF = nullptr;

	Device_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pD));
	if (nullptr == pD)
	{
		GameEngineDebug::MsgBoxError("IDXGIDevice null");
	}

	pD->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pA));
	if (nullptr == pA)
	{
		GameEngineDebug::MsgBoxError("IDXGIAdapter null");
	}

	pA->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pF));
	if (nullptr == pF)
	{
		GameEngineDebug::MsgBoxError("IDXGIFactory null");
	}
	
	if (S_OK != pF->CreateSwapChain(Device_, &ScInfo, &SwapChain_))
	{
		GameEngineDebug::MsgBoxError("SwapChain Create Error");
	}

	pF->Release();
	pA->Release();
	pD->Release();

	ID3D11Texture2D* BackBufferTexture = nullptr;
	if (S_OK != SwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBufferTexture)))
	{
		GameEngineDebug::MsgBoxError("SwapChain Texture Error");
	}

	GameEngineTextureManager::GetInst().Create("BackBuffer", BackBufferTexture);
	BackBufferTarget_ = GameEngineRenderTargetManager::GetInst().Create("BackBuffer", "BackBuffer", float4::BLUE);

	// DepthBuffer_ = GameEngineDepthBufferManager::GetInst().Create("DeviceDepthBuffer", GameEngineWindow::GetInst().GetSize());
}

void GameEngineDevice::RenderStart()
{
	// BackBufferTarget_ <= ���⿡ �׸� �༮�� ����.
	// �����
	BackBufferTarget_->Clear();
	// �����ϰ�
	BackBufferTarget_->Setting();
}

void GameEngineDevice::ShaderReset() 
{
	Context_->VSSetShader(nullptr, nullptr, 0);
	Context_->HSSetShader(nullptr, nullptr, 0);
	Context_->GSSetShader(nullptr, nullptr, 0);
	Context_->DSSetShader(nullptr, nullptr, 0);
	Context_->PSSetShader(nullptr, nullptr, 0);
}

void GameEngineDevice::ResourceReset()
{
	ShaderReset();
}

void GameEngineDevice::RenderEnd()
{
	// ȭ�鿡 �׷��� �ε�.
	HRESULT Result = SwapChain_->Present(0, 0);
	if (Result == DXGI_ERROR_DEVICE_REMOVED || Result == DXGI_ERROR_DEVICE_RESET)
	{
		int a = 0;
	}

	// ȭ�鿡 �ѷ���
	// BackBufferTarget_;
}