#pragma once
#include "GameEngineBase/GameEngineObjectNameBase.h"
#include "GameEngineDevice.h"

// Ό³Έν :
class GameEngineConstantBuffer : public GameEngineObjectNameBase
{
public:
	// constrcuter destructer
	GameEngineConstantBuffer();
	~GameEngineConstantBuffer();

	// delete Function
	GameEngineConstantBuffer(const GameEngineConstantBuffer& _Other) = delete;
	GameEngineConstantBuffer(GameEngineConstantBuffer&& _Other) noexcept = delete;
	GameEngineConstantBuffer& operator=(const GameEngineConstantBuffer& _Other) = delete;
	GameEngineConstantBuffer& operator=(GameEngineConstantBuffer&& _Other) noexcept = delete;

	inline unsigned int GetBufferSize() 
	{
		return BufferData_.ByteWidth;
	}

	inline ID3D11Buffer*& GetBuffer()
	{
		return Buffer_;
	}

	void Create(const D3D11_SHADER_BUFFER_DESC& _BufferDesc , ID3D11ShaderReflectionConstantBuffer* _VarInfo);

protected:

private:
	ID3D11Buffer* Buffer_;
	D3D11_BUFFER_DESC BufferData_;
	D3D11_MAPPED_SUBRESOURCE ResData_;
	D3D11_SHADER_BUFFER_DESC BufferDesc_;

	std::vector<D3D11_SHADER_VARIABLE_DESC> VarInfos_;

public:
	void ChangeData(const void* _Data, size_t _Size);

};

