#pragma once
#include <vector>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineDevice.h"
#include "GameEngineShader.h"

// Ό³Έν :
class GameEngineTextureSetting;
class GameEnginePixelShader : public GameEngineShader
{
	friend GameEngineShader;

public:
	// constrcuter destructer
	GameEnginePixelShader();
	~GameEnginePixelShader();

	// delete Function
	GameEnginePixelShader(const GameEnginePixelShader& _Other) = delete;
	GameEnginePixelShader(GameEnginePixelShader&& _Other) noexcept = delete;
	GameEnginePixelShader& operator=(const GameEnginePixelShader& _Other) = delete;
	GameEnginePixelShader& operator=(GameEnginePixelShader&& _Other) noexcept = delete;

	bool Create(
		const std::string& _ShaderCode,
		const std::string& _EntryPoint,
		UINT _VersionHigh = 5,
		UINT _VersionLow = 0
	);

	bool Load(
		const std::string& _Path,
		const std::string& _EntryPoint,
		UINT _VersionHigh = 5,
		UINT _VersionLow = 0
	);

	bool StringCompile();

	bool FileCompile(const std::string& _Path);

	void Setting();

	inline bool IsDeferred()
	{
		return IsDeferred_;
	}

protected:

private:
	bool IsDeferred_;

	ID3D11PixelShader* Shader_;

	void SetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) override;
	void SetTexture(const GameEngineTextureSetting* _Setting) override;
	void SetSampler(const GameEngineSamplerSetting* _Setting) override;
	void SetStructuredBuffer(const GameEngineStructuredBufferSetting* _Setting) override;

	void ReSetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) override;
	void ReSetTexture(const GameEngineTextureSetting* _Setting) override;
	void ReSetSampler(const GameEngineSamplerSetting* _Setting) override;
	void ReSetStructuredBuffer(const GameEngineStructuredBufferSetting* _Setting) override;


};

