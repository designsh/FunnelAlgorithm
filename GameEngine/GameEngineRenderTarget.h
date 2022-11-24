#pragma once
#include "GameEngineDevice.h"
#include "GameEngineShaderResHelper.h"

class GameEngineTexture;
class GameEngineRenderTarget : public GameEngineObjectNameBase
{
private:	// member Var
	static GameEngineRenderTarget* LastRenderTarget;
	static GameEngineDepthBuffer* LastDepthBuffer;

	std::vector<GameEngineTexture*> ReleaseTextures_;

	// �ؽ�ó���Լ� �������� �̴ϴ�.
	std::vector<GameEngineTexture*> Textures_;
	// std::vector<D3D11_VIEWPORT> ViewPort_;
	std::vector<ID3D11RenderTargetView*> RenderTargetViews_;
	std::vector<ID3D11RenderTargetView*> RenderTargetViewsReset_;
	std::vector<ID3D11ShaderResourceView*> ShaderResourcesViews_;

	std::vector<float4> ClearColor_;

	GameEngineDepthBuffer* DepthBuffer_;

	GameEngineRenderingPipeLine* Pipe_;
	GameEngineShaderResHelper Res_;

public:
	static GameEngineRenderTarget* GetLastRenderTarget() 
	{
		return LastRenderTarget;
	}
	static GameEngineDepthBuffer* GetLastDepthBuffer()
	{
		return LastDepthBuffer;
	}

	inline size_t GetShaderResourcesViewCount()
	{
		return ShaderResourcesViews_.size();
	}

	inline ID3D11ShaderResourceView* GetShaderResourcesView(size_t _Index)
	{
		return ShaderResourcesViews_[_Index];
	}

	inline float4 GetTextureSize(size_t _Index)
	{
		return Textures_[_Index]->GetTextureSize();
	}

	inline GameEngineTexture* GetTexture(size_t _Index)
	{
		return Textures_[_Index];
	}


	void Clear(bool _Death = true);

	// -1�̸� ���δ� �����϶�� �ǹ̰� �˴ϴ�.
	// 0~n�� �� �ε��� 1���� �����϶�� �ǹ̰� �˴ϴ�.
	void Setting(int _Index = -1);
	void Reset(int _Index = -1);

public:
	void Create(const std::string _TextureName, float4 _ClearColor);

	void Create(float4 _Scale, float4 _ClearColor);
	void Create(GameEngineTexture* _Texture, float4 _ClearColor);

	void CreateDepthBuffer(float4 _Scale);

	GameEngineDepthBuffer* GetDepthBuffer()
	{
		return DepthBuffer_;
	}

	void SetDepthBuffer(GameEngineDepthBuffer* _Depth);

public:
	GameEngineRenderTarget(); // default constructer ����Ʈ ������
	~GameEngineRenderTarget(); // default destructer ����Ʈ �Ҹ���

protected:		// delete constructer
	GameEngineRenderTarget(const GameEngineRenderTarget& _other) = delete; // default Copy constructer ����Ʈ ���������
	GameEngineRenderTarget(GameEngineRenderTarget&& _other) noexcept; // default RValue Copy constructer ����Ʈ RValue ���������

private:		//delete operator
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget& _other) = delete; // default Copy operator ����Ʈ ���� ������
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget&& _other) = delete; // default RValue Copy operator ����Ʈ RValue ���Կ�����

public:
	// ������ �׷��� �׸� ���� �� ����Ÿ���� �׸��� ��ģ��.
	void Merge(GameEngineRenderTarget* _Other, int _Index = 0);

	// ������ ���� �׷����� �װ� �����. �־��ذɷ� �ٲ۴�.
	void Copy(GameEngineRenderTarget* _Other);

};

