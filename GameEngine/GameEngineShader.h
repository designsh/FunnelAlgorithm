#pragma once
#include <set>
#include <string>
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineObjectNameBase.h>
#include "GameEngineDevice.h"
#include "GameEngine/GameEngineConstantBuffer.h"

enum class ShaderType
{
	VS,
	PS,
	MAX
};

// 설명 :
class GameEngineFile;
class GameEngineSampler;
class GameEngineTexture;
class GameEngineConstantBuffer;
class GameEngineStructuredBuffer;
class GameEngineSamplerSetting;
class GameEngineTextureSetting;
class GameEngineConstantBufferSetting;
class GameEngineStructuredBufferSetting;
class GameEngineShader : public GameEngineObjectNameBase 
{
public:
	friend class GameEngineLayOut;

	static void AutoCompile(GameEngineFile& ShaderFile);

public:
	// constrcuter destructer
	GameEngineShader(ShaderType _Type);
	virtual ~GameEngineShader() = 0;

	// delete Function
	GameEngineShader(const GameEngineShader& _Other) = delete;
	GameEngineShader(GameEngineShader&& _Other) noexcept = delete;
	GameEngineShader& operator=(const GameEngineShader& _Other) = delete;
	GameEngineShader& operator=(GameEngineShader&& _Other) noexcept = delete;

	

protected:
	UINT VersionHigh_;
	UINT VersionLow_;
	ID3DBlob* CodeBlob_;
	std::string Version_;
	std::string EntryPoint_;
	std::string Code_;
	ShaderType Type_;
	std::map<std::string, std::list<D3D11_SIGNATURE_PARAMETER_DESC>> OutPutMap;

	void SetVersion(UINT _VersionHigh, UINT _VersionLow);
	void CreateVersion(const std::string& _ShaderType);
	void SetCode(const std::string& _Code);
	void SetEntryPoint(const std::string& _EntryPoint);


public:
	unsigned int GetTypeIndex()
	{
		return static_cast<unsigned int>(Type_);
	}
	void ResCheck();

	size_t GetOutPutSize(const std::string& _Name) 
	{
		std::string Name = GameEngineString::toupper(_Name);

		return OutPutMap[Name].size();
	}

private:
	std::map<std::string, GameEngineConstantBufferSetting> ConstantBuffers_;
	std::map<std::string, GameEngineSamplerSetting> Samplers_;

	// 키 없는 맵
	std::map<std::string, GameEngineTextureSetting> Textures_;

	std::map<std::string, GameEngineStructuredBufferSetting> StructuredBuffers_;

public:
	std::map<std::string, GameEngineConstantBufferSetting>& GetConstantBuffers() 
	{
		return ConstantBuffers_;
	}

	std::map<std::string, GameEngineSamplerSetting>& GetSamplers()
	{
		return Samplers_;
	}

	std::map<std::string, GameEngineTextureSetting>& GetTextures()
	{
		return Textures_;
	}

	std::map<std::string, GameEngineStructuredBufferSetting>& GetStructuredBuffers()
	{
		return StructuredBuffers_;
	}

	virtual void SetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) = 0;
	virtual void SetTexture(const GameEngineTextureSetting* _Setting) = 0;
	virtual void SetSampler(const GameEngineSamplerSetting* _Setting) = 0;
	virtual void SetStructuredBuffer(const GameEngineStructuredBufferSetting* _Setting) = 0;

	virtual void ReSetConstantBuffers(const GameEngineConstantBufferSetting* _Setting) = 0;
	virtual void ReSetTexture(const GameEngineTextureSetting* _Setting) = 0;
	virtual void ReSetSampler(const GameEngineSamplerSetting* _Setting) = 0;
	virtual void ReSetStructuredBuffer(const GameEngineStructuredBufferSetting* _Setting) = 0;
};

