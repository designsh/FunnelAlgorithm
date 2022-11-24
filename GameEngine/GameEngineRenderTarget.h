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

	// 텍스처에게서 빌려오는 겁니다.
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

	// -1이면 전부다 세팅하라는 의미가 됩니다.
	// 0~n은 그 인덱스 1개를 세팅하라는 의미가 됩니다.
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
	GameEngineRenderTarget(); // default constructer 디폴트 생성자
	~GameEngineRenderTarget(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineRenderTarget(const GameEngineRenderTarget& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineRenderTarget(GameEngineRenderTarget&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineRenderTarget& operator=(const GameEngineRenderTarget&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

public:
	// 기존에 그려진 그림 위에 이 랜더타겟의 그림을 합친다.
	void Merge(GameEngineRenderTarget* _Other, int _Index = 0);

	// 기존에 뭐가 그려졌든 그걸 지우고. 넣어준걸로 바꾼다.
	void Copy(GameEngineRenderTarget* _Other);

};

