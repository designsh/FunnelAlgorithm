#pragma once
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineDevice.h"
#include "ThirdParty/Include/DirectXTex/DirectXTex.h"

// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineTexture : public GameEngineObjectNameBase
{
public:

	inline ID3D11DepthStencilView* GetDepthStencilView()
	{
		return DepthStencilView_;
	}

	inline ID3D11RenderTargetView* GetRenderTargetView()
	{
		return RenderTargetView_;
	}

	inline ID3D11ShaderResourceView** GetShaderResourcesView()
	{
		return &ShaderResourceView_;
	}

	ID3D11RenderTargetView* CreateRenderTargetView();
	ID3D11ShaderResourceView* CreateShaderResourceView();
	ID3D11DepthStencilView* CreateDepthStencilView();

	void Create(ID3D11Texture2D* _Texture2D);

	void Create(
		float4 _TextureSize,
		DXGI_FORMAT _Format,
		D3D11_USAGE _Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT,
		unsigned int _BindFlag = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE
	);

	void Create(D3D11_TEXTURE2D_DESC _Desc);

	void Load(const std::string& _Path);


	GameEngineTexture(); // default constructer 디폴트 생성자
	~GameEngineTexture(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineTexture(const GameEngineTexture& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineTexture(GameEngineTexture&& _other) noexcept = delete; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineTexture& operator=(const GameEngineTexture& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineTexture& operator=(const GameEngineTexture&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

	D3D11_TEXTURE2D_DESC TextureDesc_;
	ID3D11Texture2D* Texture2D_;
	ID3D11RenderTargetView* RenderTargetView_;
	ID3D11ShaderResourceView* ShaderResourceView_;
	ID3D11DepthStencilView* DepthStencilView_;
	DirectX::ScratchImage Image_;

public:
	bool IsCut();
	void Cut(int _x, int _y);
	void PushCutIndex(const float4& _Size, const float4& _Pos);
	float4 GetCutData(int _Index);

	inline float4 GetTextureSize() 
	{
		return float4(static_cast<float>(TextureDesc_.Width), static_cast<float>(TextureDesc_.Height));
	}

	float4 GetPixel(int _X, int _y);

private:
	std::vector<float4> CutList_;
};

