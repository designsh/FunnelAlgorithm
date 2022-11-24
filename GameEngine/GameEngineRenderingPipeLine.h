#pragma once
#include <GameEngine/GameEngineDevice.h>
#include <GameEngineBase/GameEngineMath.h>
#include "GameEngineShaderResHelper.h"



// 분류 : 
// 용도 : 
// 설명 : 
class GameEngineBlend;
class GameEngineShader;
class GameEngineRasterizer;
class GameEnginePixelShader;
class GameEngineIndexBuffer;
class GameEngineVertexBuffer;
class GameEngineDepthStencil;
class GameEngineVertexShader;
class GameEngineConstantBuffer;
class GameEngineRenderingPipeLine : public GameEngineObjectNameBase
{
private:	// member Var
	// IA1
	GameEngineVertexBuffer* VertexBuffer_;
	// IA1
	// GameEngineVertexShader* InputLayOutVertexShader_;
	ID3D11InputLayout* LayOut_;
	bool IsCloneLayOutCreate;
	// VS
	GameEngineVertexShader* VertexShader_;
	// IA2
	GameEngineIndexBuffer* IndexBuffer_;
	// IA2
	D3D11_PRIMITIVE_TOPOLOGY Topology_;
	// RS
	GameEngineRasterizer* Rasterizer_;
	// PS
	GameEnginePixelShader* PixelShader_;
	// Blend
	GameEngineBlend* Blend_;
	// OM
	GameEngineRenderTarget* RenderTarget_;

	GameEngineDepthStencil* DepthStencil_;

public:
	bool IsDeferred();

	inline GameEngineRasterizer* GetRasterizer() const
	{
		return Rasterizer_;
	}


	inline GameEnginePixelShader* GetPixelShader() const
	{
		return PixelShader_;
	}

	inline GameEngineVertexShader* GetVertexShader() const
	{
		return VertexShader_;
	}

	// void SetInputAssembler1InputLayOutSetting(const std::string& _Name);

	void SetInputAssembler1VertexBufferSetting(const std::string& _Name);
	void SetInputAssembler1VertexBufferSetting(GameEngineVertexBuffer* _Buffer);

	void SetVertexShader(const std::string& _Name);

	void SetInputAssembler2IndexBufferSetting(const std::string& _Name);
	void SetInputAssembler2IndexBufferSetting(GameEngineIndexBuffer* _Buffer);

	void SetInputAssembler2TopologySetting(D3D11_PRIMITIVE_TOPOLOGY Topology_);

	void SetRasterizer(const std::string& _Name);

	void SetPixelShader(const std::string& _Name);

	void SetOutputMergerBlend(const std::string& _Name);

	void SetOutputMergerDepthStencil(const std::string& _Name);

	void RenderingPipeLineSetting();

	void Rendering();

	void Reset();

	void InstanceRendering();

	GameEngineRenderingPipeLine* Clone();

	void Copy(GameEngineRenderingPipeLine* _Value);

	void RasterizerClone();


public:
	GameEngineRenderingPipeLine(); // default constructer 디폴트 생성자
	~GameEngineRenderingPipeLine(); // default destructer 디폴트 소멸자

protected:		// delete constructer
	GameEngineRenderingPipeLine(const GameEngineRenderingPipeLine& _other) = delete; // default Copy constructer 디폴트 복사생성자
	GameEngineRenderingPipeLine(GameEngineRenderingPipeLine&& _other) noexcept; // default RValue Copy constructer 디폴트 RValue 복사생성자

private:		//delete operator
	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine& _other) = delete; // default Copy operator 디폴트 대입 연산자
	GameEngineRenderingPipeLine& operator=(const GameEngineRenderingPipeLine&& _other) = delete; // default RValue Copy operator 디폴트 RValue 대입연산자

	void InputAssembler1();
	void InputAssembler2();
	void VertexShader();
	void Rasterizer();
	void PixelShader();
	void OutPutMerger();

	void CreateLayOut();
};

